#include "asset_manager.h"

#include "io/mesh_loader.h"
#include "gfx/mesh.h"
#include "io/shader_loader.h"
#include "gfx/shader.h"

AssetManager::AssetManager(void) : AssetManager(DEFAULT_ASSET_DIRECTORY)
{
}

AssetManager::AssetManager(const std::string& asset_directory) : _asset_directory(asset_directory)
{
	add_loader<Mesh>(new MeshLoader("models\\"));
	add_loader<Shader>(new ShaderLoader("shaders\\"));
}

AssetManager::~AssetManager(void)
{
	for (auto pair : _loaders)
		delete pair.second;
}

void AssetManager::set_asset_directory_path(const char* path)
{
	_asset_directory = path;

	for (auto pair : _loaders)
		pair.second->_asset_path = _asset_directory;
}