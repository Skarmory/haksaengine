#pragma once

#include <glm/glm.hpp>

#include "ecs/component.h"

struct Movement : public Component<Movement>
{
	void load(NamedVariantPack* datapack)
	{
		speed = datapack->get("speed").as_float;

		velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	glm::vec3 velocity;

	float speed;
};