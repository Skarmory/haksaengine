#pragma once

#include <string>
#include <unordered_map>

#include "globals.h"
#include "service.h"
#include "input/key.h"
#include "input/mouse.h"

class InputManager : public Service
{
public:

	HAKSAENGINE_API InputManager(void);
	HAKSAENGINE_API virtual ~InputManager(void) = default;

	// Goes over each key and mouse button and sets their states for the next update
	HAKSAENGINE_API void reset_states(void);

	// Update the values of the input devices
	HAKSAENGINE_API virtual void update(void) = 0;

	// Return a key by name
	HAKSAENGINE_API const Key& get_key(const char* name) const;

	// Return the mouse
	HAKSAENGINE_API const Mouse& get_mouse(void) const;

protected:

	HAKSAENGINE_API void set_mouse_position(float x, float y);
	HAKSAENGINE_API void set_mouse_button_state(MouseButtonType type, MouseButtonState state);
	HAKSAENGINE_API void set_mouse_scroll(float scroll);
	HAKSAENGINE_API void set_key_state(const char* key, KeyState state);

private:

	Mouse _mouse;
	std::unordered_map<std::string, Key> _key_map;
};