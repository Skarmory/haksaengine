#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "services.h"
#include "ecs/component.h"
#include "io/mdl.h"

// Component that contains render information for skinned animation entities
struct SkinnedRenderable : public Component<SkinnedRenderable>
{
	void load(NamedVariantPack* datapack) override
	{
		model = datapack->get("model").as_uint;
		shader = datapack->get("shader").as_uint;

		const MDLFile& mdl = Services::get<AssetManager>()->get_asset<MDLFile>(model);

		instanced = mdl.get_animation_texture() == nullptr ? false : true;
	}

	unsigned int model;
	unsigned int shader;
	bool instanced;

	std::vector<glm::mat4> final_bone_transforms;
	std::vector<float> geoset_alphas;
};
