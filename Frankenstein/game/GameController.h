#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum class GameState
{
	LOADING,
	MENU,
	PLAYING
};

class GameController
{
public:
	static GameController* Init(int screenWidth, int screenHeight);
	static void Cleanup();
	void Run();

	static GameController* m_GameController;
private:
	GameController(GLFWwindow* window, int screenWidth, int screenHeight);
	~GameController();

	void Update(float deltaTime);
	void Render();
	
	int m_ScreenWidth, m_ScreenHeight;
	GLFWwindow* m_GlfwWindow;
	float m_DeltaTime, m_LastFrame;
	GameState gameState;
};
