#pragma once
#ifndef CollisionManager_H
#define CollisionManager_H
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include "GameObject3D.h"

class CollisionManager {
private:
	float _dist = 0.1f;
public:
	std::vector<GameObject3D*> sceneObjects;

	static BoundingBox GetBoundingBox(const GameObject3D& object) {
		Vector3 half = { object.scale.x * 0.5f, object.scale.y * 0.5f, object.scale.z * 0.5f };
		BoundingBox box{};
		box.min = Vector3Subtract(object.position, half);
		box.max = Vector3Add(object.position, half);
		return box;
	}

	// Approximate bounding sphere radius (useful for SPHERE shape)
	static float GetBoundingSphereRadius(const GameObject3D& object) {
		// For spheres, assume scale.x is diameter; otherwise use largest half-extent.
		if (object.shapeType == SPHERE) return object.scale.x * 0.5f;
		return fmaxf(fmaxf(object.scale.x, object.scale.y), object.scale.z) * 0.5f;
	}

	// Return approximate mass (fallback if mass left as default)
	static float GetMass(const GameObject3D& object) {
		// If mass explicitly set (>0) return it, otherwise approximate from volume
		if (object.mass > 0.0f) return object.mass;
		// Approximate volume: for simplicity use box volume as fallback
		return fabsf(object.scale.x * object.scale.y * object.scale.z);
	}

	// Simple clamp helper
	static float Clampf(float v, float a, float b) {
		return fmaxf(a, fminf(b, v));
	}

	const void UpdateCollision()
	{
		for (auto& obj : sceneObjects) { obj->Update();}

		for (size_t i = 0; i < sceneObjects.size(); ++i) {
			for (size_t j = i + 1; j < sceneObjects.size(); ++j) {
				GameObject3D* a = sceneObjects[i];
				GameObject3D* b = sceneObjects[j];
				if (!a || !b) continue;
				ResolveCollision(*a, *b, 0);
				ResolveCollision(*a, *b, 1);
				ResolveCollision(*a, *b, 2);
				
				if (CheckCollisionBoxes(a->hitbox, b->hitbox))
				{
					//ResolveCollision(*a, *b, 2);
					std::cout << a->name << " collided with " << b->name << " (Box vs. Box)\n";
				}

			}
		}
	}

	void ResolveCollision(GameObject3D& object, const GameObject3D& other, int axis)
	{
		BoundingBox box1 = object.hitbox;
		BoundingBox box2 = other.hitbox;
		float dist = 0.001;
		if (CheckCollisionBoxes(box1, box2))
		{
			std::cout << "Collision Detected between " << object.name << " and " << other.name << "\n";

			if (object.linearVelocity.x > 0)
			{
				object.linearVelocity.x = 0;
				object.position.x = other.position.x - (object.scale.x / 2 + dist);
				return;
			}
			else if (object.linearVelocity.x < 0)
			{
				object.linearVelocity.x = 0;
				object.position.x = other.position.x + (object.scale.x / 2 + dist);
				return;
			}
			else if (object.linearVelocity.y > 0) {
				object.linearVelocity.y = 0;
				object.position.y = other.position.y - (object.scale.y / 2 + dist);
				return;
			}
			else if (object.linearVelocity.y < 0)
			{
				object.linearVelocity.y = 0;
				object.position.y = other.position.y + (object.scale.y / 2 + dist);
				return;
			}
			else if (object.linearVelocity.z > 0) {
				object.linearVelocity.z = 0;
				object.position.z = other.position.z - (object.scale.z / 2 + dist);
				return;
			}
			else if (object.linearVelocity.z < 0)
			{
				object.linearVelocity.z = 0;
				object.position.z = other.position.z + (object.scale.z / 2 + dist);
				return;
			}
		}
	}

		/*
	void ResolveCollision(GameObject3D& object, GameObject3D& other, int type)
	{
		if (object.objectType == STATIC && other.objectType == STATIC) return;

		const float EPS = 1e-5f;

		auto invMassOf = [](const GameObject3D& g) -> float {
			if (g.objectType == STATIC) return 0.0f;
			float m = g.GetMass();
			return (m > 0.0f) ? 1.0f / m : 0.0f;
		};

		float invM1 = invMassOf(object);
		float invM2 = invMassOf(other);
		float totalInvMass = invM1 + invM2;
		if (invM1 + invM2 <= 0.0f) return; // both immovable

		if (type == 0) {
			// Sphere vs Sphere collision resolution logic
			float box1 = object.GetBoundingSphereRadius();
			float box2 = other.GetBoundingSphereRadius();
			Vector3 n = Vector3Subtract(object.position, other.position);
			float dist = Vector3Length(n);
			float penetration = (box1 + box2) - dist;
			if (penetration <= 0.0f) return;

			Vector3 normal = (dist > EPS) ? Vector3Scale(n, 1.0f / dist) : Vector3{ 1,0,0 };

			// positional correction (split by inverse mass)
			Vector3 correctionThis = Vector3Scale(normal, -penetration * (invM1 / totalInvMass));
			Vector3 correctionOther = Vector3Scale(normal, penetration * (invM2 / totalInvMass));

			if (object.objectType == DYNAMIC) object.position = Vector3Add(object.position, correctionThis);
			if (other.objectType == DYNAMIC) other.position = Vector3Add(other.position, correctionOther);

			// relative velocity
			Vector3 rv = Vector3Subtract(other.linearVecocity, object.linearVecocity);
			float velAlongNormal = Vector3DotProduct(rv, normal);
			if (velAlongNormal > 0.0f) return; // already separating

			float e = fminf(object.restitution, other.restitution);
			float j = -(1.0f + e) * velAlongNormal / totalInvMass;

			Vector3 impulse = Vector3Scale(normal, j);

			if (object.objectType == DYNAMIC) object.linearVecocity = Vector3Subtract(object.linearVecocity, Vector3Scale(impulse, invM1));
			if (other.objectType == DYNAMIC) other.linearVecocity = Vector3Add(other.linearVecocity, Vector3Scale(impulse, invM2));
			std::cout << "Sphere vs. Sphere Resolve" << std::endl;
			return;
			// (Implementation similar to the one in GameObject3D)
		}
		else if (type == 1) {
			// Box vs Sphere collision resolution logic
			GameObject3D* sphereObj = nullptr;
			GameObject3D* boxObj = nullptr;
			bool swapped = false;
			if (object.shapeType == SPHERE) {
				sphereObj = &object;
				boxObj = &other;
			}
			else {
				sphereObj = &other;
				boxObj = &object;
				swapped = true;
			}

			BoundingBox box = boxObj->GetBoundingBox();
			Vector3 sphereCenter = sphereObj->position;
			// closest point on AABB to sphere center
			Vector3 closest{};
			closest.x = Clampf(sphereCenter.x, box.min.x, box.max.x);
			closest.y = Clampf(sphereCenter.y, box.min.y, box.max.y);
			closest.z = Clampf(sphereCenter.z, box.min.z, box.max.z);

			Vector3 n = Vector3Subtract(sphereCenter, closest);
			float dist = Vector3Length(n);
			float r = sphereObj->GetBoundingSphereRadius();
			float penetration = r - dist;
			if (penetration <= 0.0f) return;

			Vector3 normal = (dist > EPS) ? Vector3Scale(n, 1.0f / dist) : Vector3{ 1,0,0 };

			// Determine inverse masses
			float invS = (sphereObj->objectType == STATIC) ? 0.0f : 1.0f / sphereObj->GetMass();
			float invB = (boxObj->objectType == STATIC) ? 0.0f : 1.0f / boxObj->GetMass();
			float totalInv = invS + invB;
			if (totalInv <= 0.0f) return;

			// positional correction
			Vector3 corrSphere = Vector3Scale(normal, penetration * (invS / totalInv));
			Vector3 corrBox = Vector3Scale(normal, -penetration * (invB / totalInv));

			if (sphereObj->objectType == DYNAMIC) sphereObj->position = Vector3Add(sphereObj->position, corrSphere);
			if (boxObj->objectType == DYNAMIC) boxObj->position = Vector3Add(boxObj->position, corrBox);

			// impulse
			Vector3 rv = Vector3Subtract(boxObj->linearVecocity, sphereObj->linearVecocity);
			float velAlongNormal = Vector3DotProduct(rv, normal);
			if (velAlongNormal > 0.0f) return;

			float e = fminf(sphereObj->restitution, boxObj->restitution);
			float j = -(1.0f + e) * velAlongNormal / totalInv;
			Vector3 impulse = Vector3Scale(normal, j);

			if (sphereObj->objectType == DYNAMIC) sphereObj->linearVecocity = Vector3Subtract(sphereObj->linearVecocity, Vector3Scale(impulse, invS));
			if (boxObj->objectType == DYNAMIC) boxObj->linearVecocity = Vector3Add(boxObj->linearVecocity, Vector3Scale(impulse, invB));

			// done
			std::cout << "Sphere vs. Axis" << std::endl;
			return;

			// (Implementation similar to the one in GameObject3D)
		}
		else if (type == 2) {
			// Box vs Box collision resolution logic
			BoundingBox a = object.GetBoundingBox();
			BoundingBox b = other.GetBoundingBox();

			// compute overlaps
			float overlapX = fminf(a.max.x, b.max.x) - fmaxf(a.min.x, b.min.x);
			float overlapY = fminf(a.max.y, b.max.y) - fmaxf(a.min.y, b.min.y);
			float overlapZ = fminf(a.max.z, b.max.z) - fmaxf(a.min.z, b.min.z);

			// if any overlap negative -> no collision (defensive)
			if (overlapX <= 0.0f || overlapY <= 0.0f || overlapZ <= 0.0f) return;

			// find minimum axis
			float minOverlap = overlapX;
			Vector3 normal = { (object.position.x < other.position.x) ? -1.0f : 1.0f, 0, 0 };

			if (overlapY < minOverlap) { minOverlap = overlapY; normal = { 0, (object.position.y < other.position.y) ? -1.0f : 1.0f, 0 }; }
			if (overlapZ < minOverlap) { minOverlap = overlapZ; normal = { 0, 0, (object.position.z < other.position.z) ? -1.0f : 1.0f }; }

			// positional correction
			Vector3 correctionThis = Vector3Scale(normal, -minOverlap * (invM1 / totalInvMass));
			Vector3 correctionOther = Vector3Scale(normal, minOverlap * (invM2 / totalInvMass));

			if (object.objectType == DYNAMIC) object.position = Vector3Add(object.position, correctionThis);
			if (other.objectType == DYNAMIC) other.position = Vector3Add(other.position, correctionOther);

			// impulse (project velocities onto normal)
			Vector3 rv = Vector3Subtract(other.linearVecocity, object.linearVecocity);
			float velAlongNormal = Vector3DotProduct(rv, normal);
			if (velAlongNormal > 0.0f) return;

			float e = fminf(object.restitution, other.restitution);
			float j = -(1.0f + e) * velAlongNormal / totalInvMass;
			Vector3 impulse = Vector3Scale(normal, j);

			if (object.objectType == DYNAMIC) object.linearVecocity = Vector3Subtract(object.linearVecocity, Vector3Scale(impulse, invM1));
			if (other.objectType == DYNAMIC) other.linearVecocity = Vector3Add(other.linearVecocity, Vector3Scale(impulse, invM2));
			std::cout << "Axis vs. Axis Resolve" << std::endl;
			return;

			// (Implementation similar to the one in GameObject3D)
		}
	}
		*/


};



#endif
