#include "asset_manager.h"

#include <fstream>

AssetManager::AssetManager(void) : AssetManager(DEFAULT_ASSET_DIRECTORY)
{
}

AssetManager::AssetManager(const std::string& asset_directory) : _asset_directory(asset_directory)
{
	_mesh_loader = new MeshLoader(DEFAULT_ASSET_DIRECTORY + "models\\");
}

AssetManager::~AssetManager(void)
{
	delete _mesh_loader;
}

void AssetManager::set_asset_directory_path(const char* path)
{
	_asset_directory = path;
}

void AssetManager::load_asset(const char* asset_path)
{
	std::string complete_path = _asset_directory + asset_path;


}