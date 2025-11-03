#pragma once
#include <raylib.h>
#include <raymath.h>

class EditorCamera {
private:
	Vector3 lookRotation{};
	Vector2 lean{};
	float headTimer = 0.0f;
protected:
	Ray mouseRay{};
	bool lookAtMouse = false;

public:
	Camera3D camera{};
	Quaternion rotation{ 0, 0, 0 };
	Matrix matrix = MatrixIdentity();
	int speed = 10;

	void SetMouseTarget(const bool m) { lookAtMouse = m; }

	void Start() {
		camera.position = Vector3{ 0.0f, 30.0f, 30.0f };
		camera.target = Vector3{ 0.0f, 0.0f, 0.0f };
		camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
		camera.fovy = 45.0f;
		camera.projection = CAMERA_PERSPECTIVE;

	}

	void Update() {
		const Vector3 up = { 0.0f, 1.0f, 0.0f };
		const Vector3 targetOffset = Vector3(0, 0, -1);
		const float deltaTime = GetFrameTime();
		// Left and right
		Vector3 yaw = Vector3RotateByAxisAngle(targetOffset, up, lookRotation.x);

		// Clamp View up
		float maxAngleUp = Vector3Angle(up, yaw);
		maxAngleUp -= 0.001f; // prevent gimbal lock
		if (-(lookRotation.y) > maxAngleUp) { lookRotation.y = -maxAngleUp; }

		// Clamp view down
		float maxAngleDown = Vector3Angle(Vector3Negate(up), yaw);
		maxAngleDown *= -1.0f; // Downwards angle is negative
		maxAngleDown += 0.001f; // Avoid numerical errors
		if (-(lookRotation.y) < maxAngleDown) { lookRotation.y = -maxAngleDown; }

		// Up and down
		Vector3 right = Vector3Normalize(Vector3CrossProduct(yaw, up));

		// Rotate view vector around right axis
		float pitchAngle = -lookRotation.y - lean.y;
		pitchAngle = Clamp(pitchAngle, -PI / 2 + 0.0001f, PI / 2 - 0.0001f); // Clamp angle so it doesn't go past straight up or straight down
		Vector3 pitch = Vector3RotateByAxisAngle(yaw, right, pitchAngle);

		// Head animation
		// Rotate up direction around forward axis
		float headSin = sinf(headTimer * PI);
		float headCos = cosf(headTimer * PI);
		const float stepRotation = 0.01f;
		camera.up = Vector3RotateByAxisAngle(up, pitch, headSin * stepRotation + lean.x);

		// Camera BOB
		const float bobSide = 0.1f;
		const float bobUp = 0.15f;
		Vector3 bobbing = Vector3Scale(right, headSin * bobSide);
		bobbing.y = fabsf(headCos * bobUp);

		//camera.position = Vector3Add(camera.position, Vector3Scale(bobbing, walkLerp));
		camera.target = Vector3Add(camera.position, pitch);

		Ray forwardRay = { camera.position, Vector3Normalize(Vector3Subtract(camera.target, camera.position)) };

		if (IsKeyDown(KEY_W)) {
			camera.position = Vector3Add(camera.position, forwardRay.direction * speed * deltaTime);
		}
		if (IsKeyDown(KEY_S)) {
			camera.position = Vector3Add(camera.position, forwardRay.direction * -speed * deltaTime);
		}
		if (IsKeyDown(KEY_D)) {
			camera.position = Vector3Add(camera.position, right * speed * deltaTime);
		}
		if (IsKeyDown(KEY_A)) {
			camera.position = Vector3Add(camera.position, right * -speed * deltaTime);
		}
		
	}
};