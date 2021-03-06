#pragma once

#include "ecs/system.h"

class CameraControllerScript : public System 
{
public:
	CameraControllerScript(SystemOrdering order);

	void update(float delta) override;
	void on_event(Event e) override;
};