#pragma once

class Mouse
{
	friend class InputManager;

public:

	Mouse(void);

	// Returns true if mouse button one is pressed
	bool button_1_pressed(void) const;

	// Returns true if mouse button two is pressed
	bool button_2_pressed(void) const;

	// Returns true if mouse button one was just released
	bool button_1_released(void) const;

	// Returns true is mouse button two was just released
	bool button_2_released(void) const;

	// Returns the screen x position of the mouse cursor
	float x(void) const;

	// Returns the screen y position of the mouse cursor
	float y(void) const;

	// Returns the offset of the mouse x position from the previous x position
	float x_offset(void) const;

	// Returns the offset of the mouse y position from the previous y position
	float y_offset(void) const;

private:

	bool _button_1_pressed;
	bool _button_2_pressed;

	bool _button_1_released;
	bool _button_2_released;

	float _cursor_x;
	float _cursor_y;

	float _cursor_x_offset;
	float _cursor_y_offset;
};