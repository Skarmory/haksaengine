#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "ecs/component.h"

// Component that contains render information for skinned animation entities
struct SkinnedRenderable : public Component<SkinnedRenderable>
{
	void load(NamedVariantPack* datapack) override
	{
		model = datapack->get("model").as_uint;
		shader = datapack->get("shader").as_uint;
	}

	unsigned int model;
	unsigned int shader;

	std::vector<glm::mat4> final_bone_transforms;
	std::vector<float> geoset_alphas;
};
