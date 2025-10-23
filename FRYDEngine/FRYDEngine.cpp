// https://www.youtube.com/watch?v=UoAsDlUwjy0&t
//#D Imports 3:44:02
#include <iostream>
#include <raylib.h>

#include "src/Game.h"

using namespace std;

Game game;
Camera2D camera2D;
Camera3D camera3D;

Color inputInducator = RED;

Mesh cubeMesh;
Model cubeModel;
Image cubeImage;
Texture cubeTexture;
Vector3 modelPosition = { 0.0f, 0.0f, 0.0f };


static void Start() {
	game.Start();

	camera2D.target = { 0.0f, 0.0f };
	camera2D.offset = { 0.0f, 10.0f };
	camera2D.rotation = 0.0f;
	camera2D.zoom = 3.0f;

	camera3D.position = { 0.0f, 30.0f, 30.0f };
	camera3D.target = { 0.0f, 0.0f, 0.0f };
	camera3D.up = { 0.0f, 1.0f, 0.0f };
	camera3D.fovy = 45.0f;
	camera3D.projection = CAMERA_PERSPECTIVE;

	cubeMesh = GenMeshSphere(5.0f, 5.0f, 10.0f);
	cubeModel = LoadModelFromMesh(cubeMesh);
	cubeImage = GenImageGradientLinear(20, 20, 1, BLUE, GREEN);
	cubeTexture = LoadTextureFromImage(cubeImage);
	SetMaterialTexture(&cubeModel.materials[0], MATERIAL_MAP_ALBEDO, cubeTexture);
	
}

static void Update() {
	if (game.IsRunning) {
		game.Update();
	}
}

static void Draw2D() {

	DrawText("Welcome to FRYD Engine!", 190, 200, 20, LIGHTGRAY);

}

static void Draw3D() {

	modelPosition.y = sinf(GetTime()) * 5.0f;
	cubeModel.transform = MatrixTranslate(modelPosition.x, modelPosition.y, modelPosition.z);

	DrawGrid(10, 1.0f);
	DrawModel(cubeModel, modelPosition, 1.0f, GREEN);

}

int main()
{
	const int screenWidth = 1280;
	const int screenHeight = 720;
	
	InitWindow(screenWidth, screenHeight, "FRYD Engine");
	SetTargetFPS(60);
	cout << "FRYD Engine Initialized!" << endl;

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
		BeginMode3D(camera3D);
		
		Draw3D();
		
		EndMode3D();

		// Update 2D
		BeginMode2D(camera2D);

		DrawRectangle(10, 10, 20, 20, inputInducator);
		Draw2D();
		
		game.Draw();

		EndMode2D();

		// Final Draw Calls

		EndDrawing();

	}

	CloseWindow();
}
