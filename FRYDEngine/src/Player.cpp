#include "Player.h"
#include "Physics.h"

const void Player::Draw() {
	DrawRectangleV(position, scale, color);
}

void Player::Update() {
	position = Physics::ApplyGravity(position, 9.8f, GetFrameTime());
	if (IsKeyDown(KEY_RIGHT)) {
		position.x += speed;
	}
	if (IsKeyDown(KEY_LEFT)) {
		position.x -= speed;
	}
	if (IsKeyDown(KEY_UP)) {
		position.y -= speed;
	}
	if (IsKeyDown(KEY_DOWN)) {
		position.y += speed;
	}
}