#include "input/key.h"

Key::Key(void) : _name("NoKey")
{
}

Key::Key(const std::string& name) : _name(name)
{
}

const char* Key::get_name(void) const
{
	return _name.c_str();
}

bool Key::pressed(void) const
{
	return _pressed;
}

bool Key::released(void) const
{
	return _released;
}