#pragma once

#include <vector>

#include "globals.h"
#include "entity.h"
#include "event.h"

/* The system classes are responsible for processing logic on the entities and updating component data. */
/* This is the base class that all systems need to inherit from. */
class HAKSAENGINE_API System
{
public:
	virtual void update(float delta) = 0;
	virtual void on_event(Event e) = 0;

private:
	std::vector<Entity*> _entities;
};
