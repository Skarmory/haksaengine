#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#include "globals.h"

class GlfwWindow
{
	friend class GlfwInputManager;

public:

	GlfwWindow(int width, int height, const std::string& window_title);
	~GlfwWindow(void);

	bool window_close(void);
	void swap_buffers(void);

private:

	GLFWwindow* window;
};