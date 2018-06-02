#include "engine.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

Engine::Engine(void)
{

}

Engine::~Engine(void)
{
	delete game_window;

	glfwTerminate();
}

void Engine::initialise(void)
{
	glfwInit();

	glewExperimental = GL_TRUE;
	glewInit();

	game_window = new GameWindow(800, 600, "Game Application");
}