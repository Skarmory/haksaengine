#pragma once

#include <vector>

#include "globals.h"
#include "event/event_handler.h"
#include "ecs/system_ordering.h"

class SystemManager;

/* The system classes are responsible for processing logic on the entities and updating component data. */
/* This is the base class that all systems need to inherit from. */
class System : public EventHandler
{
	friend class SystemManager;

public:
	HAKSAENGINE_API System(SystemOrdering order);

	HAKSAENGINE_API virtual ~System(void) = default;

	HAKSAENGINE_API virtual void update(float delta) = 0;

protected:
	std::vector<unsigned int> _entities;

private:
	unsigned int _id;
	SystemOrdering _order;
};
