#pragma once

#include "ecs/system.h"

// A rendering logic class that processes entities with a SkinnedRenderable component
class SkinnedRenderer : public System
{
public:
	virtual void update(float delta) override;
	virtual void on_event(Event ev) override;
};