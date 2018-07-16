#pragma once

#include "ecs/render_logic_system.h"
#include "gfx/uniform_buffer_object.h"
#include "gfx/uniforms.h"

// A rendering logic class that processes entities with a SkinnedRenderable component
class SkinnedRenderer : public RenderLogicSystem
{
public:

	SkinnedRenderer(void);
	virtual void update(float delta) override;
	virtual void on_event(Event ev) override;
};