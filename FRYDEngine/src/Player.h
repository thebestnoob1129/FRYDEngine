#pragma once
#include <raylib.h>

class Player {

public:
	Vector2 position;
	Vector2 scale;
	Color color;
	int speed;

	const void Draw();
	void Update();
};