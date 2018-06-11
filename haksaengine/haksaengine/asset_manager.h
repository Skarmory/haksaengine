#pragma once

#include <string>
#include <unordered_map>
#include <typeindex>
#include <exception>

#include "io/loader.h"

class AssetManager;
class Loader;

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

	HAKSAENGINE_API void set_asset_directory_path(const char* path);
	/*HAKSAENGINE_API const char*  get_asset_directory_path(void) const;*/

	template<class C>
	unsigned int load_asset(const char* asset_name)
	{
		unsigned int  id = std::hash<std::string>{}(std::string(asset_name));

		// Check if we've loaded this asset already
		if (_assets.find(id) != _assets.end())
			return id;

		std::type_index tid = typeid(C);

		// Not loaded, check if we have a loader for it
		if (_loaders.find(tid) != _loaders.end())
		{
			// A loader exists so load this asset
			Asset* loaded = _loaders[tid]->load(asset_name);
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

		return id;
	}

	template<class C>
	void add_loader(Loader* loader)
	{
		loader->_asset_path = _asset_directory;
		_loaders[typeid(C)] = loader;
	}

private:
	const std::string DEFAULT_ASSET_DIRECTORY = "assets\\";
	std::string _asset_directory;

	std::unordered_map<std::type_index, Loader*> _loaders;
	std::unordered_map<unsigned int, Asset*> _assets;
};
