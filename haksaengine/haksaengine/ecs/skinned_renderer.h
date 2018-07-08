#pragma once

#include "ecs/system.h"


class SkinnedRenderer : public System
{
public:
	virtual void update(float delta) override;
	virtual void on_event(Event ev) override;
};