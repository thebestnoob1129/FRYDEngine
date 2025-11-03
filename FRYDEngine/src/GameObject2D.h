#pragma once
#include <raylib.h>
#include <raymath.h>
#include <Physics.h>

#ifndef GAMEOBJECT2D_H
#define GAMEOBJECT2D_H

inline Physics globalPhysics2D;

class GameObject2D {
protected:
	Ray forward{};
	Ray right{};
	bool isRayDraw = false;
	Rectangle rect;
public:
	std::string name{};
	int objectID{};

	Vector2 position{ 0,0 };
	Vector2 scale{ 1,1 };
	Vector3 rotation{ 0,0,0 };
	
	Vector2 linearVelocity = Vector2Zero();
	Vector2 acceleration = Vector2Zero();
	Color color = WHITE;

	void Draw() {
		DrawRectangleRec(rect, color);
	};

	void Update() {


		rect.x = position.x - (scale.x / 2);
		rect.y = position.y - (scale.y / 2);
		rect.width = scale.x;
		rect.height = scale.y;
	};

}


#endif // !GAMEOBJECT2D_H