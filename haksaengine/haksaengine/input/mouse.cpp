#include "input/mouse.h"

Mouse::Mouse(void) 
	: _button_1_pressed(false), _button_2_pressed(false), _button_1_released(false), _button_2_released(false),
	  _cursor_x(0.0f), _cursor_y(0.0f), _cursor_x_offset(0.0f), _cursor_y_offset(0.0f)
{
}

bool Mouse::button_1_pressed(void) const
{
	return _button_1_pressed;
}

bool Mouse::button_2_pressed(void) const
{
	return _button_2_pressed;
}

bool Mouse::button_1_released(void) const
{
	return _button_1_released;
}

bool Mouse::button_2_released(void) const
{
	return _button_2_released;
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