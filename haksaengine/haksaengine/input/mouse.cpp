#include "input/mouse.h"

Mouse::Mouse(void) 
	: _button_1({ false, false }), _button_2({ false,false }),
	  _cursor_x(0.0f), _cursor_y(0.0f),
	  _cursor_x_offset(0.0f), _cursor_y_offset(0.0f),
	  _scroll(0.0f)
{
}

bool Mouse::pressed(MouseButtonType button) const
{
	if (button == MouseButtonType::Button_1)
		return _button_1.pressed;
	else if (button == MouseButtonType::Button_2)
		return _button_2.pressed;

	return false;
}

bool Mouse::released(MouseButtonType button) const
{
	if (button == MouseButtonType::Button_1)
		return _button_1.released;
	else if (button == MouseButtonType::Button_2)
		return _button_2.released;

	return false;
}

float Mouse::x(void) const
{
	return _cursor_x;
}

float Mouse::y(void) const
{
	return _cursor_y;
}

float Mouse::x_offset(void) const
{
	return _cursor_x_offset;
}

float Mouse::y_offset(void) const
{
	return _cursor_y_offset;
}

float Mouse::scroll(void) const
{
	return _scroll;
}

MouseButton* Mouse::_get_button(MouseButtonType type)
{
	switch (type)
	{
		case MouseButtonType::Button_1: return &_button_1;
		case MouseButtonType::Button_2: return &_button_2;
		default: return nullptr;
	}
}