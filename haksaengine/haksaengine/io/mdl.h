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

private:
	std::vector<Mesh*> _meshes;
	std::vector<Texture*> _textures;
	std::vector<MDLData> _data;
	std::vector<Bone> _bones;
	std::vector<Animation> _animations;

	friend class MDLLoader;
	friend class Renderer;
};