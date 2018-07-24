#include "input/glfw_input_manager.h"

#include <GLFW/glfw3.h>

#include "services.h"

GlfwInputManager::GlfwInputManager(const GlfwWindow& window)
{
	glfwSetKeyCallback(window.window, (GLFWkeyfun)key_callback);
	glfwSetCursorPosCallback(window.window, (GLFWcursorposfun)mouse_move_callback);
	glfwSetMouseButtonCallback(window.window, (GLFWmousebuttonfun)mouse_button_callback);
}

void GlfwInputManager::update(void)
{
	glfwPollEvents();
}

std::string GlfwInputManager::_key_to_string(int key)
{
	switch (key)
	{
		// Alphabetical
		case GLFW_KEY_A: return "A";
		case GLFW_KEY_B: return "B";
		case GLFW_KEY_C: return "C";
		case GLFW_KEY_D: return "D";
		case GLFW_KEY_E: return "E";
		case GLFW_KEY_F: return "F";
		case GLFW_KEY_G: return "G";
		case GLFW_KEY_H: return "H";
		case GLFW_KEY_I: return "I";
		case GLFW_KEY_J: return "J";
		case GLFW_KEY_K: return "K";
		case GLFW_KEY_L: return "L";
		case GLFW_KEY_M: return "M";
		case GLFW_KEY_N: return "N";
		case GLFW_KEY_O: return "O";
		case GLFW_KEY_P: return "P";
		case GLFW_KEY_Q: return "Q";
		case GLFW_KEY_R: return "R";
		case GLFW_KEY_S: return "S";
		case GLFW_KEY_T: return "T";
		case GLFW_KEY_U: return "U";
		case GLFW_KEY_V: return "V";
		case GLFW_KEY_W: return "W";
		case GLFW_KEY_X: return "X";
		case GLFW_KEY_Y: return "Y";
		case GLFW_KEY_Z: return "Z";

		// Numeric
		case GLFW_KEY_0: return "0";
		case GLFW_KEY_1: return "1";
		case GLFW_KEY_2: return "2";
		case GLFW_KEY_3: return "3";
		case GLFW_KEY_4: return "4";
		case GLFW_KEY_5: return "5";
		case GLFW_KEY_6: return "6";
		case GLFW_KEY_7: return "7";
		case GLFW_KEY_8: return "8";
		case GLFW_KEY_9: return "9";

		// Other
		case GLFW_KEY_ESCAPE: return "Esc";
		case GLFW_KEY_LEFT_CONTROL: case GLFW_KEY_RIGHT_CONTROL: return "Ctrl";
		case GLFW_KEY_LEFT_ALT:     case GLFW_KEY_RIGHT_ALT:     return "Alt";
		case GLFW_KEY_LEFT_SHIFT:   case GLFW_KEY_RIGHT_SHIFT:   return "Shift";

		// Null
		default: return "NoKey";
	}
}

MouseButtonType GlfwInputManager::_glfw_button_convert(int button)
{
	switch (button)
	{
		case GLFW_MOUSE_BUTTON_1: return MouseButtonType::Button_1;
		case GLFW_MOUSE_BUTTON_2: return MouseButtonType::Button_2;
		default: return MouseButtonType::None;
	}
}

void GlfwInputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GlfwInputManager* input = static_cast<GlfwInputManager*>(Services::get().get_input_manager());

	std::string keystr = input->_key_to_string(key);

	if (keystr == "NoKey")
		return;

	if (action == GLFW_PRESS)
		input->set_key_state(keystr.c_str(), KeyState::Pressed);

	else if(action == GLFW_RELEASE)
		input->set_key_state(keystr.c_str(), KeyState::Released);
}

void GlfwInputManager::mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
	GlfwInputManager* input = static_cast<GlfwInputManager*>(Services::get().get_input_manager());

	input->set_mouse_position(xpos, ypos);
}

void GlfwInputManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	GlfwInputManager* input = static_cast<GlfwInputManager*>(Services::get().get_input_manager());

	MouseButtonType type = input->_glfw_button_convert(button);

	if (action == GLFW_PRESS)
		input->set_mouse_button_state(type, MouseButtonState::Pressed);

	else if (action == GLFW_RELEASE)
		input->set_mouse_button_state(type, MouseButtonState::Released);
}