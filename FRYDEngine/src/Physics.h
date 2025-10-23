#include <raylib.h>
#include <raymath.h>
#include <iostream>

class Physics {
	public:

	static Vector2 ApplyGravity(const Vector2& position, float gravity, float deltaTime) {
		return { position.x, position.y + gravity * deltaTime };
	}
	static bool CheckCollision(const Rectangle& rect1, const Rectangle& rect2) {
		return (rect1.x < rect2.x + rect2.width &&
				rect1.x + rect1.width > rect2.x &&
				rect1.y < rect2.y + rect2.height &&
				rect1.y + rect1.height > rect2.y);
	}
};