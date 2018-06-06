#include "window.h"

GameWindow::GameWindow(int width, int height, const std::string& window_title)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(width, height, window_title.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(window);
}

GameWindow::~GameWindow(void)
{
	glfwDestroyWindow(window);
	window = nullptr;
}

bool GameWindow::window_close(void)
{
	return glfwWindowShouldClose(window);
}