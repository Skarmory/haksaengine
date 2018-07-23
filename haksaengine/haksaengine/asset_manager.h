#pragma once

#include <string>
#include <unordered_map>
#include <typeindex>
#include <exception>

#include "service.h"
#include "event/event.h"
#include "io/loader.h"

class AssetManager;
class Loader;

// Base class that all assets need to inherit from
class Asset
{
public:
	virtual ~Asset(void)
	{}

private:
	unsigned int id;
	std::string name;

	friend class AssetManager;
};

// Class that holds all the assets loaded into the game
class AssetManager : public Service
{
public:
	AssetManager(void);
	AssetManager(const std::string& asset_directory);
	~AssetManager(void);

	// Sets the asset directory path. All asset data needs to be in some subdirectory of this
	HAKSAENGINE_API void set_asset_directory_path(const char* path);

	// Get an asset by its id
	HAKSAENGINE_API Asset& get_asset(unsigned int asset_id) const;

	// Get an asset by its id and cast it to the given type.
	template<class AssetType>
	AssetType& get_asset(unsigned int asset_id) const
	{
		return static_cast<AssetType&>(get_asset(asset_id));
	}

	// Load an asset of a given type by its filename
	template<class C>
	unsigned int load_asset(const char* asset_name)
	{
		return _load_asset(asset_name, typeid(C));
	}

	template<class AssetType>
	AssetType& load_and_get_asset(const char* asset_name)
	{
		return get_asset<AssetType>(_load_asset(asset_name, typeid(AssetType)));
	}

	// Add a loader for a particular type of asset so the asset manager can load it
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
	std::unordered_map<std::type_index, Event> _load_event_map;
	std::unordered_map<unsigned int, Asset*> _assets;

	HAKSAENGINE_API unsigned int _load_asset(const std::string& asset_name, std::type_index type);
};
