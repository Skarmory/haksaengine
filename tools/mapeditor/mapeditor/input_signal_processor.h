#pragma once

#include "ecs/system.h"

class InputSignalProcessor : public System
{
public:
	InputSignalProcessor(SystemOrdering order);
	void update(float delta) override;
	void on_event(Event e) override;
};