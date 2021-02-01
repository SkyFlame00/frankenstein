#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

	
	int m_ScreenWidth, m_ScreenHeight;
	GLFWwindow* m_GlfwWindow;
	float m_DeltaTime, m_LastFrame;
};
