#pragma once

#include <string>
#include <unordered_map>
#include <list>

#include "globals.h"
#include "event/event.h"
#include "event/event_handler.h"

class EventHandler;

class EventManager
{
public:
	HAKSAENGINE_API void subscribe(const char* event_type, EventHandler* system);
	HAKSAENGINE_API void dispatch(Event e);

private:
	std::unordered_map<std::string, std::list<EventHandler*>> _event_receivers;
};