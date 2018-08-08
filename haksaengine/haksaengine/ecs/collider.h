#pragma once

#include "ecs/component.h"
#include "scene/shapes.h"

class Collider : public Component<Collider>
{
	void load(NamedVariantPack* datapack)
	{
		glm::vec3 min;
		glm::vec3 max;

		min.x = datapack->get("min_x").as_float;
		min.y = datapack->get("min_y").as_float;
		min.z = datapack->get("min_z").as_float;

		max.x = datapack->get("max_x").as_float;
		max.y = datapack->get("max_y").as_float;
		max.z = datapack->get("max_z").as_float;

		aabb = AABB(min, max);
	}

	AABB aabb;
};