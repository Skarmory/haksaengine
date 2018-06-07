#include "asset_manager.h"

AssetManager::AssetManager(void) : asset_path(DEFAULT_ASSET_DIRECTORY)
{
}

void AssetManager::set_asset_directory_path(const char* path)
{
	asset_path = path;
}

void AssetManager::load_asset(const char* asset_path)
{
	
}