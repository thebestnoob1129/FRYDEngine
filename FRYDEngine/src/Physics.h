#pragma once
#include <raylib.h>
#include <raymath.h>
#include <iostream>

class Physics {
	public:
		Vector3 gravity;
	
	static Vector3 ApplyGravity(const Vector3& position, float gravity, float deltaTime) {
		return { position.x, position.y + gravity * deltaTime, position.z };
	}
	static Vector2 ApplyGravity(const Vector2& position, float gravity, float deltaTime) {
		return { position.x, position.y + gravity * deltaTime };
	}

	static Vector2 ApplyVelocity(const Vector2& position, const Vector2& velocity, float deltaTime) {
		return { position.x + velocity.x * deltaTime, position.y + velocity.y * deltaTime };
	}
	static Vector3 ApplyVelocity(const Vector3& position, const Vector3& velocity, float deltaTime) {
		return { position.x + velocity.x * deltaTime, position.y + velocity.y * deltaTime, position.z + velocity.z * deltaTime };
	}

	static bool CheckCollision(const Rectangle& rect1, const Rectangle& rect2) {
		return (rect1.x < rect2.x + rect2.width &&
				rect1.x + rect1.width > rect2.x &&
				rect1.y < rect2.y + rect2.height &&
				rect1.y + rect1.height > rect2.y);
	}
};
