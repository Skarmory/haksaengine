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

private:
	GLFWwindow* window;
};