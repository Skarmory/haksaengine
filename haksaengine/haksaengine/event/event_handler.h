#pragma once

#include "event/event.h"

class EventHandler
{
public:
	virtual void on_event(Event ev) = 0;
};