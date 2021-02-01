#include "GameController.h"


#include <iostream>

GameController* GameController::m_GameController;

GameController* GameController::Init(int screenWidth, int screenHeight)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return m_GameController = new GameController(window, screenWidth, screenHeight);
}

void GameController::Cleanup()
{
	delete m_GameController;
}

GameController::GameController(GLFWwindow* window, int screenWidth, int screenHeight)
    : m_GlfwWindow(window), m_ScreenWidth(screenWidth), m_ScreenHeight(screenHeight)
{
}

GameController::~GameController()
{
}

void GameController::Run()
{
    while (!glfwWindowShouldClose(m_GlfwWindow))
    {
        float currentFrame = glfwGetTime();
        m_DeltaTime = currentFrame - m_LastFrame;
        m_LastFrame = currentFrame;

        glfwSwapBuffers(m_GlfwWindow);
        glfwPollEvents();
    }
    glfwTerminate();
}
