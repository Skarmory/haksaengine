#pragma once

#include "ecs/system.h"

class CameraController : public System
{
public:

	CameraController(SystemOrdering order);

	virtual void update(float delta) override;
	virtual void on_event(Event ev) override;

};