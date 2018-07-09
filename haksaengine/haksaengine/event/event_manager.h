#pragma once

#include <string>
#include <unordered_map>
#include <list>

#include "globals.h"
#include "event/event.h"
#include "event/event_handler.h"

class EventHandler;

// Class that deals with all event data. EventHandlers can register with the EventManager for specific event types.
class EventManager
{
public:

	// Subscribe an event handler with an event type. Subscribed handlers will receive all events of that type
	HAKSAENGINE_API void subscribe(const char* event_type, EventHandler* handler);

	// Dispatch an event. All event handlers subscribed with this event type will receive a copy of it.
	HAKSAENGINE_API void dispatch(Event e);

private:
	std::unordered_map<std::string, std::list<EventHandler*>> _event_receivers;
};