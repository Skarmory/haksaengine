#pragma once

#include "globals.h"
#include <string>

enum class KeyState
{
	Pressed = 0,
	Released = 1,
};

class Key
{
	friend class InputManager;

public:

	Key(void);
	explicit Key(const std::string& name);

	const char* get_name(void) const;

	HAKSAENGINE_API bool pressed(void) const;
	HAKSAENGINE_API bool released(void) const;

private:

	std::string _name;

	bool _pressed;
	bool _released;
};