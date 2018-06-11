#pragma once

#include <string>

#include "io/mesh_loader.h"

class AssetManager;

class Asset
{
	unsigned int id;
	const char* name;

	friend class AssetManager;
};

class AssetManager
{
public:
	AssetManager(void);
	AssetManager(const std::string& asset_directory);
	~AssetManager(void);

	void HAKSAENGINE_API set_asset_directory_path(const char* path);
	void HAKSAENGINE_API load_asset(const char* asset_path);
	
private:

	const std::string DEFAULT_ASSET_DIRECTORY = "..\\..\\assets\\";

	std::string _asset_directory;
	MeshLoader* _mesh_loader;
};
