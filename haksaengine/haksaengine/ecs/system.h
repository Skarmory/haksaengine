#pragma once

#include <vector>

#include "globals.h"
#include "entity_manager.h"
#include "event_manager.h"
#include "event.h"

class EntityManager;
class EventManager;
struct Event;

/* The system classes are responsible for processing logic on the entities and updating component data. */
/* This is the base class that all systems need to inherit from. */
class HAKSAENGINE_API System
{
public:
	System(void);

	virtual ~System(void) = default;

	virtual void update(float delta) = 0;
	virtual void on_event(Event e) = 0;

protected:
	std::vector<unsigned int> _entities;
};
