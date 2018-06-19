#pragma once

#include <vector>

#include "asset_manager.h"
#include "gfx/mesh.h"
#include "gfx/texture.h"

struct MDLData
{
	unsigned int mesh_id;
	unsigned int texture_id;
};

class MDLFile : public Asset
{
public:
	MDLFile(void);
	~MDLFile(void);

private:
	std::vector<Mesh*> _meshes;
	std::vector<Texture*> _textures;
	std::vector<MDLData> _data;

	friend class MDLLoader;
	friend class Renderer;
};