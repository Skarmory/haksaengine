#pragma once

#include "ecs/system.h"

class UnitPlacer : public System
{
public:
	UnitPlacer(SystemOrdering order);

	void update(float delta) override;
	void on_event(Event e) override;
};