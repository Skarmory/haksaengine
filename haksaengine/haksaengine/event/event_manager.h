#pragma once

#include <string>
#include <unordered_map>
#include <list>

#include "globals.h"
#include "event.h"
#include "ecs/system.h"

class System;

class EventManager
{
public:
	HAKSAENGINE_API void subscribe(const char* event_type, System* system);
	HAKSAENGINE_API void dispatch(Event e);

private:
	std::unordered_map<std::string, std::list<System*>> _event_receivers;
};