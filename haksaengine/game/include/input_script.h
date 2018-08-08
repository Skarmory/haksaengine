#pragma once

#include "ecs/system.h"

class InputScript : public System
{
public:

	InputScript(SystemOrdering order);

	void update(float delta) override;
	void on_event(Event e) override;
};