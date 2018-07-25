#pragma once

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
	bool pressed(MouseButtonType button) const;

	// Returns true if the given mouse button was just released
	bool released(MouseButtonType button) const;

	// Returns the screen x position of the mouse cursor
	float x(void) const;

	// Returns the screen y position of the mouse cursor
	float y(void) const;

	// Returns the offset of the mouse x position from the previous x position
	float x_offset(void) const;

	// Returns the offset of the mouse y position from the previous y position
	float y_offset(void) const;

	// Returns the amount scrolled since last update
	float scroll(void) const;

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