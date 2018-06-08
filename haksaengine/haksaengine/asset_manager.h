#pragma once

#include "gfx/mesh_loader.h"

class AssetManager
{
public:
	AssetManager(void);

	void HAKSAENGINE_API set_asset_directory_path(const char* path);
	void HAKSAENGINE_API load_asset(const char* asset_path);
	
private:

	const std::string DEFAULT_ASSET_DIRECTORY = "..\\..\\assets";

	std::string asset_path;
	MeshLoader _mesh_loader;
};
