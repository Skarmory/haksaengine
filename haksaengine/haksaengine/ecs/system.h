#pragma once

#include <vector>

#include "globals.h"
#include "entity_manager.h"
#include "event/event_handler.h"
#include "event/event.h"

struct Event;

/* The system classes are responsible for processing logic on the entities and updating component data. */
/* This is the base class that all systems need to inherit from. */
class System : public EventHandler
{
public:
	HAKSAENGINE_API System(void);

	HAKSAENGINE_API virtual ~System(void) = default;

	HAKSAENGINE_API virtual void update(float delta) = 0;

protected:
	std::vector<unsigned int> _entities;
};
