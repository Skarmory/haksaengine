#pragma once

#include "globals.h"
#include "ecs/system.h"
#include "event/event.h"

class RenderingSystem : public System
{
public:
	RenderingSystem(void);
	virtual void update(float delta) override;
	virtual void on_event(Event ev) override;
};