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

	// Update the values of the input devices
	virtual void update(void);

	// Return a key by name
	const Key& get_key(const char* name) const;

	// Return the mouse
	const Mouse& get_mouse(void) const;

private:

	Mouse _mouse;
	std::unordered_map<std::string, Key> _key_map;
};