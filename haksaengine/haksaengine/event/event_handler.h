#pragma once

#include "event/event.h"

// Base class that any class that needs to receive events from the engine EventManager needs to inherit from
class EventHandler
{
public:
	virtual void on_event(Event ev) = 0;
};