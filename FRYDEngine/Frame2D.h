#pragma once
#ifndef FRAME2D_H
#define FRAME2D_H

#include <raylib.h>
#include <raymath.h>
#include <string>

class Frame2D {

protected:
	Rectangle rect{ 0, 0, 0, 0 };

public:
	Vector2 position{ rect.x, rect.y };
	Vector2 size{ rect.width , rect.height };
	
	Vector3 vec1;
	Vector3 vec2;

	float value1;
	float value2;

	Frame2D() {
		position = { rect.x, rect.y };
		size = { rect.width , rect.height };
	};
	~Frame2D() {

	};
	
	void Draw() {

		// Draw frame based on position and size
		DrawRectangleRec(rect, WHITE);
		DrawText("Frame2D", (int)position.x + 5, (int)position.y + 5, 10, BLACK);
		DrawText(("Position: " + std::to_string((int)position.x) + ", " + std::to_string((int)position.y)).c_str(), (int)position.x + 5, (int)position.y + 20, 8, BLACK);
	};


};

#endif