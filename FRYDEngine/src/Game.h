#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include "Player.h"

class Game {


public:
	bool IsRunning = true;

	//Game();
	//~Game();
	void Start();
	void Update();
	const void Draw();
};
