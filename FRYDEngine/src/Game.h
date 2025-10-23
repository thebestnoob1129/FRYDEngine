#pragma once
#include <raylib.h>
#include <raymath.h>
#include <iostream>

#include "Player.h"
#include "Physics.h"

Player player;

GameObject3D plane;

class Game {


public:
	bool IsRunning = true;

	//Game();
	//~Game();
	void Start() {

		plane.scale = { 10, 1, 10 };
		plane.position = { 0, -10, 0 };
		plane.color = DARKGREEN;
		plane.objectType = STATIC;

		plane.Render(PLANE);
		
		player.objectType = STATIC;
		player.shapeType = MESH;
		player.position = { 0, 1, 0 };
		player.scale = Vector3One();
		player.color = RED;
		player.speed = 5;

		player.Render(CUBE);

	}
	const void Update() {
		if (!IsRunning) return;
		player.Update();
	}
	void Draw2D() {

	}
	void Draw3D() {
		plane.Draw();
		player.Draw();
	}
};
