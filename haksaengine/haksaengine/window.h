#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#include "globals.h"

class GameWindow
{
public:
	GameWindow(int width, int height, const std::string& window_title);
	~GameWindow(void);

	bool window_close(void);
	void swap_buffers(void);

private:
	GLFWwindow* window;
};