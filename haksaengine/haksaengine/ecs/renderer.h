#pragma once

#include "globals.h"
#include "event/event.h"
#include "ecs/system.h"

class Renderer : public System
{
public:
	Renderer(void);
	virtual void update(float delta) override;
	virtual void on_event(Event ev) override;
};