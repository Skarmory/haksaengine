#pragma once

#include <glm/glm.hpp>

#include "ecs/component.h"

struct LocationComponent : public Component
{
	glm::vec3 position;
};