#pragma once

#include <string>

class Key
{
	friend class InputManager;

public:

	Key(void);
	explicit Key(const std::string& name);

	const char* get_name(void) const;

	bool pressed(void) const;
	bool released(void) const;

private:

	std::string _name;

	bool _pressed;
	bool _released;
};