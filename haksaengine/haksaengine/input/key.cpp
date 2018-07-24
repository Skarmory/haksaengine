#include "input/key.h"

Key::Key(void) : Key("NoKey")
{
}

Key::Key(const std::string& name) : _name(name), _pressed(false), _released(false)
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