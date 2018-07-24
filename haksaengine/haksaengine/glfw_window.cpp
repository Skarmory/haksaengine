#include "glfw_window.h"

GlfwWindow::GlfwWindow(int width, int height, const std::string& window_title)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(width, height, window_title.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
}

GlfwWindow::~GlfwWindow(void)
{
	glfwDestroyWindow(window);
	window = nullptr;
}

bool GlfwWindow::window_close(void)
{
	return glfwWindowShouldClose(window);
}

void GlfwWindow::swap_buffers(void)
{
	glfwSwapBuffers(window);
}