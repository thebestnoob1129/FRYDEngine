#pragma once
#ifndef UICamera_H
#define UICamera_H
#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "GameObject3D.h"

class UICamera {

public:
	Camera2D camera;
	std::vector<GameObject3D*> sceneObjects;

	void Start() {
		camera.target = { 0.0f, 0.0f };
		camera.offset = { 0.0f, 10.0f };
		camera.rotation = 0.0f;
		camera.zoom = 3.0f;
	}

	void Update() {
		sceneObjects = GameObject3D::sceneObjects;
	}

	void Draw() {
		for (auto* obj : sceneObjects) {
			// Object Data
		}
		
		for (int i = 0; i < sceneObjects.size(); i++) {
			DrawRectangle(20, 20 * (i * 30), 50, 20, WHITE);
		}
	}

};
#endif