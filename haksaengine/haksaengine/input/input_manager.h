#pragma once

#include <string>
#include <unordered_map>

#include "service.h"
#include "input/key.h"
#include "input/mouse.h"

class InputManager : public Service
{
public:

	InputManager(void);
	virtual ~InputManager(void) = default;

	// Goes over each key and mouse button and sets their states for the next update
	void reset_states(void);

	// Update the values of the input devices
	virtual void update(void) = 0;

	// Return a key by name
	const Key& get_key(const char* name) const;

	// Return the mouse
	const Mouse& get_mouse(void) const;

protected:

	void set_mouse_position(float x, float y);
	void set_mouse_button_state(MouseButtonType type, MouseButtonState state);
	void set_key_state(const char* key, KeyState state);

private:

	Mouse _mouse;
	std::unordered_map<std::string, Key> _key_map;
};