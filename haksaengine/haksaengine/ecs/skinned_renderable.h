#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "ecs/component.h"

struct SkinnedRenderable : public Component<SkinnedRenderable>
{
	void load(NamedVariantPack* datapack) override
	{
		model = datapack->get("model").as_uint;
		shader = datapack->get("shader").as_uint;
	}

	unsigned int model;
	unsigned int shader;

	std::vector<glm::mat4> frame_final_bone_transforms;
};
