#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "input/input_manager.h"
#include "glfw_window.h"

class GlfwInputManager : public InputManager
{
public:

	GlfwInputManager(const GlfwWindow& window);

	// Update the input state using GLFW logic
	virtual void update(void) override;

private:

	const GlfwWindow* _window;

	// Convert GLFW key code to string
	std::string _key_to_string(int key);

	// Convert GLFW button code to MouseButtonType enum
	MouseButtonType _glfw_button_convert(int button);

	// Callback function for GLFW key events
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	// Callback function for GLFW mouse move events
	static void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);

	// Callback function for GLFW mouse button press events
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	// Callback function for GLFW mouse scroll events
	static void mouse_scroll_callback(GLFWwindow* window, double x, double y);
};