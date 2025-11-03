#pragma once
#ifndef Game_H
#define Game_H

#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <string>
#include <vector>

#include "Player.h"
#include "CollisionManager.h"
#include "GameObject3D.h"
#include "EditorCamera.h"
#include "UICamera.h"

inline Player player;
inline CollisionManager collision_manager;

inline GameObject3D plane;
inline GameObject3D cube;

class Game {
private:
	std::vector<GameObject3D*> gameObjects;
protected:
	Vector3 gravity{0 ,0, 0};
	Vector3 centerWorldPosition{0, 0, 0};
	Camera2D camera2D{};
	Camera3D camera3D{};
	UICamera uiCamera{};
	EditorCamera editorCamera{};

public:
	bool IsRunning = true;

	void set_camera(const Camera2D& cam) { camera2D = cam; }
	void set_camera(const Camera3D& cam) { camera3D = cam; }
	void set_camera(const UICamera& cam) { uiCamera = cam; }
	void set_camera(const EditorCamera& cam) { editorCamera = cam; }
	Camera2D GetCamera2D() const { return camera2D; }
	Camera3D GetCamera3D() const { return camera3D; }
	UICamera GetUiCamera() const { return uiCamera; }
	EditorCamera GetEditorCamera() const { return editorCamera; }

	void Start() {

		player.name = "Player";
		player.shapeType = CUBE;
		player.objectType = DYNAMIC;
		player.position = Vector3{ 0, 0, 0 };
		player.scale = Vector3One();
		player.color = RED;
		player.speed = 5;
		player.Render(CUBE);

		plane.name = "Ground Plane";
		plane.objectType = STATIC;
		plane.shapeType = PLANE;
		plane.position = Vector3{ 0, -3, 0 };
		plane.scale = Vector3{ 10, 1, 10 };
		plane.color = DARKGREEN;
		plane.Render(PLANE);

		cube.name = "Static Cube";
		cube.objectType = STATIC;
		cube.shapeType = CUBE;
		cube.position = Vector3{ 5, 0, 5 };
		cube.scale = Vector3One();
		cube.color = BLUE;
		cube.Render(CUBE);

		for (int i = 0; i < 8; i++)
		{
			
			GameObject3D obj{};
			obj.name = "Row " + std::to_string(i);
			obj.objectType = STATIC;
			obj.shapeType = CUBE;
			obj.position = Vector3{ 0, -1, (float)(i * 8) };
			obj.scale = Vector3{ 8, 1, 8 };
			obj.color = BROWN;
			obj.Render(CUBE);
			for (int j = 0; j < 8; j++)
			{
				GameObject3D obj2{};
				obj2.name = "Column " + std::to_string(j);
				obj2.objectType = STATIC;
				obj2.shapeType = CUBE;
				obj2.position = Vector3{ (float)(j), 0, (float)(i * 8) };
				obj2.scale = Vector3{ 1, 1, 8 };
				obj2.color = DARKGRAY;
				obj2.Render(CUBE);
			}
		}

	}
	void Update() {
		gameObjects = GameObject3D::sceneObjects;
		collision_manager.sceneObjects = GameObject3D::sceneObjects;
		collision_manager.UpdateCollision();
		if (!IsRunning) return;

		
	}
	void Draw2D() {
		DrawText(TextFormat("%i", gameObjects.size()), 100, 10, 8, WHITE);
		DrawText(TextFormat("Position X:%02i", player.position.x), 10, 60, 8, WHITE);
		DrawText(TextFormat("Position Y:%02i", player.position.y), 10, 80, 8, WHITE);
		DrawText(TextFormat("Position Z:%02i", player.position.z), 10, 100, 8, WHITE);\

		DrawText(TextFormat("Velocity X:%02i", player.linearVelocity.x), 10, 120, 8, WHITE);
		DrawText(TextFormat("Velocity Y:%02i", player.linearVelocity.y), 10, 140, 8, WHITE);
		DrawText(TextFormat("Velocity Z:%02i", player.linearVelocity.z), 10, 160, 8, WHITE);
	}
	void Draw3D() {
		//plane.Draw();
		//player.Draw();
		//cube.Draw();
		for (auto& obj : gameObjects) {
			obj->Draw();
		}
		
	}

};
#endif // !FRYDEngine_Game_H