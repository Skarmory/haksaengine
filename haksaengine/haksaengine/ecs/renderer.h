#pragma once

#include "globals.h"
#include "event/event.h"
#include "ecs/system.h"

// Basic renderer. Renders entities that have Renderable components
class Renderer : public System
{
public:
	Renderer(void);
	virtual void update(float delta) override;
	virtual void on_event(Event ev) override;
};