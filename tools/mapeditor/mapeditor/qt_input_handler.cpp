#include "qt_input_handler.h"

QtInputHandler::QtInputHandler(void) : _scroll(0.0f)
{
}

void QtInputHandler::update(void)
{
	while (!events.empty())
	{
		QtInputEvent* e = &events.front();

		switch (e->type)
		{
		case QtInputEvent::MOUSE_MOVE:
		{
			set_mouse_position(e->data[0].as_int, e->data[1].as_int);
			break;
		}

		case QtInputEvent::MOUSE_PRESS:
		{
			set_mouse_button_state(static_cast<MouseButtonType>(e->data[0].as_int), MouseButtonState::Pressed);
			break;
		}

		case QtInputEvent::MOUSE_RELEASE:
		{
			set_mouse_button_state(static_cast<MouseButtonType>(e->data[0].as_int), MouseButtonState::Released);
			break;
		}

		case QtInputEvent::MOUSE_SCROLL:
		{
			_scroll = e->data[0].as_int;
			break;
		}
		}

		events.pop();
	}

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
	QtInputEvent e;
	e.type = QtInputEvent::MOUSE_MOVE;
	e.data.reserve(2);

	Variant v;
	v.type = Variant::INTEGER;
	v.as_int = event->x();

	e.data.push_back(v);

	v.as_int = event->y();
	e.data.push_back(v);

	std::lock_guard<std::mutex> lg(mu);
	events.push(e);
}

void QtInputHandler::process_mouse_pressed(QMouseEvent* event)
{
	MouseButtonType type = _qt_mouse_button_convert(event->button());

	QtInputEvent e;
	e.type = QtInputEvent::MOUSE_PRESS;
	e.data.reserve(1);

	Variant v;
	v.type = Variant::INTEGER;
	v.as_int = static_cast<int>(type);

	e.data.push_back(v);

	std::lock_guard<std::mutex> lg(mu);
	events.push(e);
}

void QtInputHandler::process_mouse_released(QMouseEvent* event)
{
	MouseButtonType type = _qt_mouse_button_convert(event->button());

	QtInputEvent e;
	e.type = QtInputEvent::MOUSE_RELEASE;
	e.data.reserve(1);

	Variant v;
	v.type = Variant::INTEGER;
	v.as_int = static_cast<int>(type);

	e.data.push_back(v);

	std::lock_guard<std::mutex> lg(mu);
	events.push(e);
}

void QtInputHandler::process_mouse_scroll(QWheelEvent* event)
{
	auto adelta = event->angleDelta();

	QtInputEvent e;
	e.type = QtInputEvent::MOUSE_SCROLL;
	e.data.reserve(1);

	Variant v;
	v.type = Variant::INTEGER;
	v.as_int = static_cast<int>(adelta.y());

	e.data.push_back(v);

	std::lock_guard<std::mutex> lg(mu);
	events.push(e);
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