#pragma once
#include <raylib.h>
#include <raymath.h>

class EditorCamera {

public:
	Camera3D camera{};
	Quaternion rotation{ 0, 0, 0 };
	Vector3 forward{0, 0, 1};
	Matrix matrix = MatrixIdentity();

	void Start() {
		camera.position = { 0.0f, 30.0f, 30.0f };
		camera.target = { 0.0f, 0.0f, 0.0f };
		camera.up = { 0.0f, 1.0f, 0.0f };
		camera.fovy = 45.0f;
		camera.projection = CAMERA_PERSPECTIVE;

		rotation = QuaternionFromEuler(
			DEG2RAD * 45,
			DEG2RAD * 90,
			DEG2RAD * 0
		);

		//matrix = MatrixMultiply(matrix, MatrixRotateY(DEG2RAD * 90) -> Rotates 90 Degrees

	};

	void Update() {
		float deltaTime = GetFrameTime();

		// Camera Rotation

		forward = Vector3{
			matrix.m8,
			matrix.m9,
			matrix.m10
		};
		forward = Vector3Normalize(forward);

		// Camera Movement

		if (IsKeyDown(KEY_W)) {
			camera.position.z -= 10.0f * deltaTime;
		}
		if (IsKeyDown(KEY_S)) {
			camera.position.z += 10.0f * deltaTime;
		}
		if (IsKeyDown(KEY_A)) {
			camera.position.x -= 10.0f * deltaTime;
		}
		if (IsKeyDown(KEY_D)) {
			camera.position.x += 10.0f * deltaTime;
		}
	}


};