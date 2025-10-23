#pragma once
#include <raylib.h>
#include <raymath.h>

class UICamera {

public:
	Camera2D camera;

	void Start() {
		camera.target = { 0.0f, 0.0f };
		camera.offset = { 0.0f, 10.0f };
		camera.rotation = 0.0f;
		camera.zoom = 3.0f;
	}

	void Update() {

	}

};