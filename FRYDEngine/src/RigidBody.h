#pragma once
#include <raylib.h>
#include <raymath.h>
#include <iostream>

#include "Physics.h"

inline Physics globalPhysics;

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

enum ObjectType {
	STATIC,
	DYNAMIC
};

enum ShapeType {
	CUBE,
	CYLINDER,
	CAPSULE,
	MESH,
	SPHERE,
	PLANE
};

class RigidBody {
private:
	int InstanceID{};
protected:
	Ray forward{};
	Ray right{};
	bool isRayDraw = false;
	Vector3 linearVelocity = Vector3Zero();
	Vector3 angularVelocity = Vector3Zero();
	Vector3 acceleration = Vector3Zero();
	Vector3 position = Vector3Zero();
public:
	Mesh mesh{};
	Model model{};
	ObjectType objectType{};
	ShapeType shapeType{};
	
	bool is2D = false;

	// Physics properties used for response
	float friction = 0.5f;
	float mass = 1.0f;         // approximate mass (non-zero for dynamic objects)
	float restitution = 0.2f;  // coefficient of restitution [0..1] (inelastic -> elastic)
	
	RigidBody() {
		model = LoadModelFromMesh(mesh);
		InstanceID = GetRandomValue(1000, 100000);
	}
	~RigidBody() {
		UnloadModel(model);
	}
	Vector3 ApplyForce(Vector3 force) {
		Vector3 acceleration = Vector3Scale(force, 1.0f / mass);
		//gameObject.linearVelocity = Vector3Add(linearVelocity, acceleration);
		return acceleration;
	}
	Vector3 ApplyImpulse(Vector3 impulse) {
		Vector3 deltaV = Vector3Scale(impulse, 1.0f / mass);
		//gameObject->linearVelocity = Vector3Add(linearVelocity, deltaV);
		return deltaV;
	}
	int GetID() const {
		return InstanceID;
	}

	void Draw() {
		DrawLine3D(forward.position, forward.position + Vector3Scale(forward.direction, 2.0f), RED);
		//DrawLine3D(position, position + linearVelocity, GREEN);
	}
	void Update(Vector3 position) {
		model.transform = MatrixTranslate(position.x, position.y, position.z);
		gameObject->linearVelocity = Vector3Scale(gameObject->linearVelocity, friction);
		
		if (objectType == DYNAMIC) {
			position = globalPhysics.ApplyGravity(position, globalPhysics.gravity.y, GetFrameTime());
			position = globalPhysics.ApplyVelocity(position, linearVelocity, GetFrameTime());
		}
		return position;
	}
	void Update(Vector2 position) {
		// 2D Update logic can be implemented here
		model.transform = MatrixTranslate(position.x, position.y, position.z);
		linearVelocity = Vector2Scale(linearVelocity, friction);
		if (objectType == DYNAMIC) {
			position = globalPhysics.ApplyGravity(position, globalPhysics.gravity.y, GetFrameTime());
			position = globalPhysics.ApplyVelocity(position, linearVelocity, GetFrameTime());
		}
		return position;
	}
};

#endif // !RIGIDBODY_H