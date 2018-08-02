#pragma once

#include "globals.h"

enum MouseButtonType
{
	Button_1 = 0,
	Button_2 = 1,
	None     = 2
};

enum class MouseButtonState
{
	Pressed  = 0,
	Released = 1
};

struct MouseButton
{
	bool pressed;
	bool released;
};

class Mouse
{
	friend class InputManager;

public:

	Mouse(void);

	// Returns true if the given mouse button is pressed
	HAKSAENGINE_API bool pressed(MouseButtonType button) const;

	// Returns true if the given mouse button was just released
	HAKSAENGINE_API bool released(MouseButtonType button) const;

	// Returns the screen x position of the mouse cursor
	HAKSAENGINE_API float x(void) const;

	// Returns the screen y position of the mouse cursor
	HAKSAENGINE_API float y(void) const;

	// Returns the offset of the mouse x position from the previous x position
	HAKSAENGINE_API float x_offset(void) const;

	// Returns the offset of the mouse y position from the previous y position
	HAKSAENGINE_API float y_offset(void) const;

	// Returns the amount scrolled since last update
	HAKSAENGINE_API float scroll(void) const;

private:

	// Return mouse button
	MouseButton* _get_button(MouseButtonType type);

private:

	MouseButton _button_1;
	MouseButton _button_2;

	float _cursor_x;
	float _cursor_y;

	float _cursor_x_offset;
	float _cursor_y_offset;

	float _scroll;
};