#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject3D.h"

// Render Player as object first the move object
class Player : public GameObject3D {

public:
	Color color = WHITE;
	int speed = 5;
	ShapeType shapeType = CUBE;
	ObjectType objectType = DYNAMIC;

	void Update() override{
		GameObject3D::Update();
		
		if (IsKeyDown(KEY_RIGHT)) { linearVelocity.x += speed * GetFrameTime(); }
		if (IsKeyDown(KEY_LEFT)) { linearVelocity.x -= speed * GetFrameTime();	}
		if (IsKeyDown(KEY_UP)) { linearVelocity.z -= speed * GetFrameTime(); }
		if (IsKeyDown(KEY_DOWN)) { linearVelocity.z += speed * GetFrameTime();	}

	};
};
#endif