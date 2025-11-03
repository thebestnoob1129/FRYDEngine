#pragma once
#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#include "RigidBody.h"
#include "CollisionManager.h"

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameObject {
private:
	int instanceId;
	int objectId;
protected:
	Ray forward{};
	Ray right{};
	bool isRayDraw = false;
	bool showHitbox = false;

	Image image{};
	Texture texture{};
	Mesh mesh{};
	Model model{};

	bool isRendered = false;
	bool isActive = false;
	bool is2D = false;

public:
	std::string name{};
	Color color = WHITE;

	Vector3 position = Vector3Zero();
	Vector3 scale = Vector3One();
	Quaternion rotation = Vector4Zero();
	Matrix matrix = MatrixIdentity();

	BoundingBox hitbox{};
	Rigidbody rigidBody{};
	Mesh mesh;

	inline static std::vector<GameObject*> sceneObjects;

	GameObject(bool dim) {
		if (dim) { is2D = true; }
		else { is2D = false; }

		instanceId = GetRandomValue(0, INT16_MAX);

		for (int i = 0; i < sceneObjects.size(); i++) {
			if (sceneObjects[i]->GetObjectId() >= objectId) {
				objectId = sceneObjects[i]->GetObjectId() + 1;
			}
		}
		if (std::ranges::find(sceneObjects.begin(), sceneObjects.end(), this) == sceneObjects.end()) { return; }
		else { sceneObjects.push_back(this); }

	}

	virtual ~GameObject() {
		if (is2D) {
			UnloadTexture(texture);
			UnloadImage(image);
		}
		else
		{
			UnloadModel(model);
			UnloadMesh(mesh);
		}
		auto it = std::ranges::find(sceneObjects.begin(), sceneObjects.end(), this);
		if (it != sceneObjects.end()) sceneObjects.erase(it);
	}

	virtual void Render() {
		if (isRendered) return;
		
		if (is2D) {
			texture = LoadTextureFromImage(image);
			DrawTexture(texture, (int)position.x, (int)position.y, color);
		}
		else {
			model = LoadModelFromMesh(mesh);
			DrawModel(model, position, 1, color);
		}
		isRendered = true;
		std::cout << "Rendered Object: " << name.c_str() << std::endl;

	}

	virtual void Update() {
		// Update logic can be implemented here
		rigidBody.Update(position);

		if (is2D) {
			// 2D Update logic can be implemented here
		}
		else {
			model.transform = MatrixTranslate(position.x, position.y, position.z);
			hitbox.min = Vector3Subtract(position * 2, scale * 0.5f);
			hitbox.max = Vector3Add(position * 2, scale * 0.5f);
		}
	}

	virtual void Draw() {
		if (is2D) {
			DrawTexture(texture, (int)position.x, (int)position.y, color);
		}
		else {
			if (showHitbox) { DrawBoundingBox(hitbox, RED); }
			DrawModel(model, position, 1, color);
		}
	}

	float GetMass() { return CollisionManager::GetMass(*this); }

	int GetObjectId() { return objectId; }
	int GetInstanceID() { return instanceId; }
};

#endif GAMEOBJECT_H