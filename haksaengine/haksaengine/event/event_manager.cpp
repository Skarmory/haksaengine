#include "event_manager.h"

// Subscribe a system to receive an event type
void EventManager::subscribe(const char* event_type, EventHandler* system)
{
	_event_receivers[event_type].push_back(system);
}

// Send the event to all registered handlers of the event
void EventManager::dispatch(Event e)
{
	std::list<EventHandler*>& systems = _event_receivers[e.event_type];

	for (auto sys : systems)
	{
		sys->on_event(e);
	}
}