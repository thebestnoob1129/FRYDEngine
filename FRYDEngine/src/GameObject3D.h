#pragma once
#ifndef GameObject3D_H
#define GameObject3D_H

#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

#include "Physics.h"
#include "CollisionManager.h"

inline Physics globalPhysics;

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

class GameObject3D {

protected:
	Ray forward{};
	Ray backward{};
	Ray right{};
	Ray left{};
	bool isRayDraw = false;

public:
	std::string name{};
	int objectID{};

	Vector3 position{ 0, 0, 0 };
	Quaternion rotation{ 0, 0, 0 };
	Vector3 scale{ 1, 1, 1 };
	Color color = WHITE;
	int slices = 1;

	BoundingBox hitbox{};

	Image image{};
	Texture texture{};
	Mesh mesh{};
	Model model{};
	Material material{};

	Vector3 linearVelocity = Vector3Zero();
	float friction = 0.5f;
	Matrix matrix = MatrixIdentity();
	ObjectType objectType{};
	ShapeType shapeType{};

	// Physics properties used for response
	float mass = 1.0f;         // approximate mass (non-zero for dynamic objects)
	float restitution = 0.2f;  // coefficient of restitution [0..1] (inelastic -> elastic)

	bool isRendered = false;

	// Global registry of all GameObject3D instances
	inline static std::vector<GameObject3D*> sceneObjects;

	// Register on construction

	GameObject3D() {
		for (size_t i = 0; i < sceneObjects.size(); ++i) {
			if (sceneObjects[i]->objectID >= objectID)
				objectID = sceneObjects[i]->objectID + 1;
		}
		if (ranges::find(sceneObjects.begin(), sceneObjects.end(), this) == sceneObjects.end())
		{
			return;
		}
		else
		{
			sceneObjects.push_back(this);
		}

	}
	// Unregister on destruction
	virtual ~GameObject3D() {
		
		UnloadModel(model);
		auto it = ranges::find(sceneObjects.begin(), sceneObjects.end(), this);
		if (it != sceneObjects.end()) sceneObjects.erase(it);
	}

	// Render variants
	virtual void Render(ShapeType type) {
		switch (shapeType) {

		case CUBE: mesh = GenMeshCube(scale.x, scale.y, scale.z); break;
		case CYLINDER: mesh = GenMeshCylinder(scale.x, scale.y, slices); break;
		case SPHERE: mesh = GenMeshSphere(scale.x, 1, slices); break;
		case MESH: model = LoadModelFromMesh(mesh); break;
		case PLANE: mesh = GenMeshPlane(scale.x, scale.z, 1, 1); break;

		default:
			DrawText(name.c_str(), (int)position.x, (int)position.y, 8, color);
			break;
		}

		if (!isRendered) {
			image = GenImageColor(20, 20, color);
			texture = LoadTextureFromImage(image);
			model = LoadModelFromMesh(mesh);
			isRendered = true;
			std::cout << "Rendered Shape" << std::endl;
			sceneObjects.push_back(this);
		}

	}
	virtual void Render(Mesh _mesh) {
		if (!isRendered) {
			image = GenImageColor(20, 20, color);
			texture = LoadTextureFromImage(image);
			model = LoadModelFromMesh(_mesh);
			isRendered = true;
			std::cout << "Rendered Mesh " << std::endl;
			sceneObjects.push_back(this);
		}
	}


	virtual void Update() {
		if (objectType == DYNAMIC) {
			position = globalPhysics.ApplyGravity(position, globalPhysics.gravity.y, GetFrameTime());
			position = globalPhysics.ApplyVelocity(position, linearVelocity, GetFrameTime());
		}
		model.transform = MatrixTranslate(position.x, position.y, position.z);
		linearVelocity = Vector3Scale(linearVelocity, friction);
		position += linearVelocity;

		hitbox.min = Vector3Subtract(position * 2, scale * 0.5f);
		hitbox.max = Vector3Add(position * 2, scale * 0.5f);
		
		forward = { position * 2, Vector3{ 0, 0, 1 } };
		backward = { position * 2, Vector3{ 0, 0, -1 } };
		right = { position * 2, Vector3{ 1, 0, 0 } };
		left = { position * 2, Vector3{ -1, 0, 0 } };

	};

	virtual void Draw() {
		DrawText(TextFormat("ID:%i", objectID), (int)10, (int)20 * objectID, 8, WHITE);
		DrawBoundingBox(hitbox, RED);
		DrawModel(model, position, 1, color);

		if (isRayDraw)
		{
			DrawRay(forward, GREEN);
			DrawRay(backward, YELLOW);
			DrawRay(right, RED);
			DrawRay(left, RED);
		}

	}

};
#endif // !GameObject3D_H