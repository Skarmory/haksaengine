#pragma once

#include <string>
#include <unordered_map>
#include <list>

#include "globals.h"
#include "event.h"
#include "ecs/system.h"

class System;

class HAKSAENGINE_API EventManager
{
public:
	void subscribe(const std::string& event_type, System* system);
	void dispatch(Event e);

private:
	std::unordered_map<std::string, std::list<System*>> _event_receivers;
};