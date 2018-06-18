#include "asset_manager.h"

#include "services.h"

#include "io/mesh_loader.h"
#include "gfx/mesh.h"
#include "io/shader_loader.h"
#include "gfx/shader.h"
#include "io/texture_loader.h"
#include "gfx/texture.h"
#include "io/blueprint_loader.h"
#include "io/blueprint.h"

AssetManager::AssetManager(void) : AssetManager(DEFAULT_ASSET_DIRECTORY)
{
}

AssetManager::AssetManager(const std::string& asset_directory) : _asset_directory(asset_directory)
{
	_loaders.reserve(5);
	_load_event_map.reserve(5);

	add_loader<Mesh>(new MeshLoader("models\\"));
	add_loader<Shader>(new ShaderLoader("shaders\\"));
	add_loader<Blueprint>(new BlueprintLoader("blueprints\\"));
	add_loader<Texture>(new TextureLoader("textures\\"));

	// Setup Events so they can be quickly retrieved and dispatched and not have to constantly rebuild them.
	Variant v;
	Event evt;
	v.type = Variant::Type::UNSIGNEDINT;
	evt.arguments.push_back(v);

	evt.event_type = "AssetMeshLoaded";
	_load_event_map[typeid(Mesh)] = evt;

	evt.event_type = "AssetShaderLoaded";
	_load_event_map[typeid(Shader)] = evt;

	evt.event_type = "AssetTextureLoaded";
	_load_event_map[typeid(Texture)] = evt;

	evt.event_type = "AssetBlueprintLoaded";
	_load_event_map[typeid(Blueprint)] = evt;
}

AssetManager::~AssetManager(void)
{
	// Destruct all the loaded assets
	for (auto id_asset_pair : _assets)
		delete id_asset_pair.second;

	// Destruct all the asset loaders
	for (auto id_loader_pair : _loaders)
		delete id_loader_pair.second;
}

void AssetManager::set_asset_directory_path(const char* path)
{
	_asset_directory = path;

	for (auto pair : _loaders)
		pair.second->_asset_path = _asset_directory;
}

const Asset& AssetManager::get_asset(unsigned int asset_id) const
{
	return *_assets.at(asset_id);
}

unsigned int AssetManager::_load_asset(const std::string& asset_name, std::type_index type)
{
	unsigned int  id = std::hash<std::string>{}(std::string(asset_name));

	// Check if we've loaded this asset already
	if (_assets.find(id) != _assets.end())
		return id;

	// Not loaded, check if we have a loader for it
	if (_loaders.find(type) != _loaders.end())
	{
		// A loader exists so load this asset
		Asset* loaded = _loaders[type]->load(asset_name);
		loaded->id = id;
		loaded->name = asset_name;

		_assets[id] = loaded;
	}
	else
	{
		// No loader, throw an exception because the game probably won't work
		//throw std::runtime_error("Error trying to load file: " + std::string(asset_name) + ". Loader for type " + std::string(tid.name) + " not found. Make sure a loader for this has been added to the AssetManager.");
		throw std::runtime_error("Failed to load asset");
	}

	Event ev = _load_event_map[type];
	ev.arguments[0].as_uint = id;

	Services::get().get_event_manager()->dispatch(ev);

	return id;
}