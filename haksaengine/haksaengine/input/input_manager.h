#pragma once

#include <string>
#include <unordered_map>

#include "service.h"
#include "input/key.h"

class InputManager : public Service
{
public:
	InputManager(void);

	virtual void update(void);

	const Key& get_key(const char* name) const;

private:

	std::unordered_map<std::string, Key> _key_map;
};