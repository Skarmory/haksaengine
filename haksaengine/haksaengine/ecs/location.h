#pragma once

#include "ecs/component.h"
#include "transform.h"

struct LocationComponent : public Component
{
	Transform transform;
};