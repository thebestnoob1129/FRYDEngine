// https://www.youtube.com/watch?v=UoAsDlUwjy0&t
//3D Imports 3:44:02
using namespace std;

#include <iostream>
#include <raylib.h>

#include "src/Game.h"
#include "src/EditorCamera.h"
#include "src/UICamera.h"

Game game;
UICamera uiCamera;
EditorCamera editorCamera;

Color inputInducator = RED;

static void Start() {

	game.Start();
	uiCamera.Start();
	editorCamera.Start();
	
}

static void Update() {
	if (game.IsRunning) {
		game.Update();
		uiCamera.Update();
		editorCamera.Update();
	}
}

static void Draw2D() {

	DrawText("Welcome to FRYD Engine!", 10, 200, 8, LIGHTGRAY);
	DrawRectangle(10, 10, 20, 20, inputInducator);
	game.Draw2D();
	
}

static void Draw3D() {

	DrawGrid(10, 1.0f);
	game.Draw3D();
}

int main()
{
	const int screenWidth = 1280;
	const int screenHeight = 720;
	
	InitWindow(screenWidth, screenHeight, "FRYD Engine");
	SetTargetFPS(60);

	Start();

	// Main game loop
	while (!WindowShouldClose()) {

		Update();
		
		ClearBackground(BLACK);
		BeginDrawing();

		// Input Indicator
		if (GetKeyPressed()) { inputInducator = GREEN;	}
		else { inputInducator = RED; }


		// Update 3D
		BeginMode3D(editorCamera.camera);
		
		Draw3D();
		
		EndMode3D();

		// Update 2D
		BeginMode2D(uiCamera.camera);

		Draw2D();
		
		EndMode2D();

		// Final Draw Calls

		EndDrawing();

	}

	CloseWindow();
}
