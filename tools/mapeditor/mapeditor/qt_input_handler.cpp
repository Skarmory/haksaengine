#include "qt_input_handler.h"

QtInputHandler::QtInputHandler(void) : _scroll(0.0f)
{
}

void QtInputHandler::update(void)
{
	if (_scroll != 0.0f)
	{
		set_mouse_scroll(_scroll);
		_scroll = 0.0f;
	}
}

void QtInputHandler::process_key(void)
{
	/*std::string keystr = input->_key_to_string(key);

	if (keystr == "NoKey")
		return;

	if (action == GLFW_PRESS)
		input->set_key_state(keystr.c_str(), KeyState::Pressed);

	else if (action == GLFW_RELEASE)
		input->set_key_state(keystr.c_str(), KeyState::Released);*/
}

void QtInputHandler::process_mouse_move(QMouseEvent* event)
{
	set_mouse_position(event->x(), event->y());
}

void QtInputHandler::process_mouse_pressed(QMouseEvent* event)
{
	MouseButtonType type = _qt_mouse_button_convert(event->button());

	set_mouse_button_state(type, MouseButtonState::Pressed);
}

void QtInputHandler::process_mouse_released(QMouseEvent* event)
{
	MouseButtonType type = _qt_mouse_button_convert(event->button());

	set_mouse_button_state(type, MouseButtonState::Released);
}

void QtInputHandler::process_mouse_scroll(QWheelEvent* event)
{
	auto delta = event->pixelDelta();
	auto adelta = event->angleDelta();

	//set_mouse_scroll(adelta.y());
	_scroll = adelta.y();
}

MouseButtonType QtInputHandler::_qt_mouse_button_convert(Qt::MouseButton button)
{
	switch (button)
	{
		case Qt::MouseButton::LeftButton: return MouseButtonType::Button_1;
		case Qt::MouseButton::RightButton: return MouseButtonType::Button_2;
		default: return MouseButtonType::None;
	}
}