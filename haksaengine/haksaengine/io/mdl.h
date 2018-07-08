#pragma once

#include <vector>

#include "asset_manager.h"
#include "gfx/mesh.h"
#include "gfx/texture.h"
#include "anim/bone.h"
#include "anim/animation.h"

// Pairs mdl file data together
struct MDLData
{
	unsigned int mesh_id;
	unsigned int texture_id;
};

// Contains all the data from a .MDL file
class MDLFile : public Asset
{
public:
	MDLFile(void);
	~MDLFile(void);

	// Return const reference to vector of all animations
	const std::vector<Animation>& get_animations(void) const
	{
		return _animations;
	}

	// Return const reference to animation by name
	const Animation* get_animation(const std::string& animation_name)
	{
		for (int i = 0; i < _animations.size(); i++)
		{
			if (_animations[i].name == animation_name)
				return &_animations[i];
		}

		return nullptr;
	}

	// Return const reference to vector of all bones
	const std::vector<Bone>& get_bones(void) const
	{
		return _bones;
	}

	// Return const reference to MDLData vector
	const std::vector<MDLData>& get_data(void) const
	{
		return _data;
	}

	// Return pointer to const Mesh if mesh exists at given index. Else return nullptr
	const Mesh* get_mesh(int index) const
	{
		if (index > _meshes.size())
			return nullptr;

		return _meshes[index];
	}

	// Return point to const Texture if texture exists at given index. Else return nullptr
	const Texture* get_texture(int index) const
	{
		if (index > _textures.size())
			return nullptr;

		return _textures[index];
	}

private:
	std::vector<Mesh*> _meshes;
	std::vector<Texture*> _textures;
	std::vector<MDLData> _data;
	std::vector<Bone> _bones;
	std::vector<Animation> _animations;

	friend class MDLLoader;
};