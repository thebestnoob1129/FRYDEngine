#pragma once
#include <raylib.h>
#include <raymath.h>
#include <iostream>

#include "Physics.h"

Physics globalPhysics;

enum ObjectType {
	STATIC,
	DYNAMIC
};

enum ShapeType {
	CUBE,
	CYLINDER,
	CAPSULE,
	MESH,
	MODEL,
	SPHERE,
	PLANE
};

class GameObject3D {

public:
	char name{};
	int objectID{};
	
	Vector3 position{ 0, 0, 0 };
	Quaternion rotation{ 0, 0, 0 };
	Vector3 scale{ 1, 1, 1 };
	Color color = WHITE;
	int slices = 1;

	BoundingBox hitbox;

	Image image{};
	Texture texture{};
	Mesh mesh{};
	Model model{};
	Material material{};

	Vector3 linearVecocity{ 0, 0, 0 };
	Matrix matrix = MatrixIdentity();
	ObjectType objectType{};
	ShapeType shapeType{};

	bool isRendered = false;

	virtual void Render(ShapeType type) {
		cout << "Rendered Shape" << endl;
		switch (shapeType) {

			case CUBE: mesh = GenMeshCube(scale.x, scale.y, scale.z); break;
			case CYLINDER: mesh = GenMeshCylinder(scale.x, scale.y, slices); break;
			case SPHERE: mesh = GenMeshSphere(scale.x, 1, slices); break;
			case MESH: model = LoadModelFromMesh(mesh); break;
			case PLANE: mesh = GenMeshPlane(scale.x, scale.y, 1, 1); break;

		default:
			DrawText(&name, (int)position.x, (int)position.y, 8, color);
			break;
		}
		
		if (!isRendered) {
			image = GenImageColor(20, 20, color);
			texture = LoadTextureFromImage(image);
			model = LoadModelFromMesh(mesh);
			isRendered = true;
		}

		hitbox.min = position;
		hitbox.max = position + scale;

	}
	virtual void Render(Mesh meshh) {
		cout << "Rendered Mesh " << endl;
		if (!isRendered) {
			image = GenImageColor(20, 20, color);
			texture = LoadTextureFromImage(image);
			model = LoadModelFromMesh(mesh);
			isRendered = true;
		}

		hitbox.min = position;
		hitbox.max = position + scale;
	}


	virtual void Update() {
		if (objectType == DYNAMIC) {
			position = globalPhysics.ApplyGravity(position, globalPhysics.gravity.y, GetFrameTime());
			position = globalPhysics.ApplyVelocity(position, linearVecocity, GetFrameTime());
		}
	};
	virtual void Draw() {
		DrawModel(model, position, 1, color);
	}

	virtual void Destroy() {
		UnloadModel(model);
		UnloadMesh(mesh);
		UnloadTexture(texture);
		UnloadImage(image);
		std::cout << "Destroyed: " + name << std::endl;
	};

	virtual bool CheckCollision(BoundingBox collider) {
		return CheckCollisionBoxes(hitbox, collider);
	}
};