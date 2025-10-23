#pragma once
#include "GameObject3D.h"
Physics physics;
// Render Player as object first the move object
class Player : public GameObject3D {

public:
	Color color = WHITE;
	int speed = 5;

	void Update() override{
		//GameObject3D::Update();
		model.transform = MatrixTranslate(position.x, position.y, position.z);
		//position = physics.ApplyGravity(position, 9.8f, GetFrameTime());
		
		if (IsKeyDown(KEY_RIGHT)) { position.x += speed; }
		if (IsKeyDown(KEY_LEFT)) { position.x -= speed;	}
		if (IsKeyDown(KEY_UP)) { position.y -= speed; }
		if (IsKeyDown(KEY_DOWN)) { position.y += speed;	}
	};
};