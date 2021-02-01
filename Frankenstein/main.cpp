#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game/GameController.h"

int main()
{
	GameController* game = GameController::Init(800, 600);

	if (!game)
	{
		return -1;
	}

	game->Run();

	GameController::Cleanup();

	return 0;
}
