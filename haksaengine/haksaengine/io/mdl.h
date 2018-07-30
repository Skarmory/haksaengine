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
	friend class MDLLoader;

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

	// Gets ID of an animation in relation to this MDL
	// Returns -1 if animation not found
	int get_animation_id(const std::string& animation_name)
	{
		for (int i = 0; i < _animations.size(); i++)
		{
			if (_animations[i].name == animation_name)
				return i;
		}

		return -1;
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

	// Return mesh vector as const reference
	const std::vector<Mesh*>& get_meshes(void) const
	{
		return _meshes;
	}

	// Return pointer to const Mesh if mesh exists at given index. Else return nullptr
	const Mesh* get_mesh(int index) const
	{
		if (index > _meshes.size())
			return nullptr;

		return _meshes[index];
	}

	// Return pointer to const Texture if texture exists at given index. Else return nullptr
	const Texture* get_texture(int index) const
	{
		if (index > _textures.size())
			return nullptr;

		return _textures[index];
	}

	// Return pointer to const Texture that contains animation data if it has been created
	const Texture* get_animation_texture(void) const
	{
		return _animation_texture;
	}

	// Expand animation data into a GPU texture
	void create_animation_texture(void);

private:

	// Helper function for expanding animation data
	void _process_bone_hierarchy(const BonePoseNode* node, float time, const std::vector<BonePoseNode>& nodes, const std::vector<Bone>& bones, std::vector<glm::mat4>& final_transforms, glm::mat4 parent_transform);

private:

	std::vector<Mesh*> _meshes;
	std::vector<Texture*> _textures;
	std::vector<MDLData> _data;
	std::vector<Bone> _bones;
	std::vector<Animation> _animations;
	Texture* _animation_texture;
};