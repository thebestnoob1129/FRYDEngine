#include "Game.h"
#include "Physics.h"

Player player;
Physics physics;

void Game::Start() {
	player.position = { 100, 100 };
	player.scale = { 50, 50 };
	player.color = RED;
	player.speed = 5;
}

void Game::Update() {
	if (!IsRunning) return;
	player.Update();
}

const void Game::Draw() {
	player.Draw();
}