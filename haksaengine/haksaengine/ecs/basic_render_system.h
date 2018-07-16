#pragma once

#include "globals.h"
#include "event/event.h"
#include "ecs/render_logic_system.h"

// Basic renderer. Renders entities that have Renderable components
class BasicRenderSystem : public RenderLogicSystem
{
public:
	BasicRenderSystem(void);
	virtual void update(float delta) override;
	virtual void on_event(Event ev) override;
};