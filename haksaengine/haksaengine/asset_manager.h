#pragma once

#include <string>
#include <unordered_map>
#include <typeindex>
#include <exception>

#include "event/event.h"
#include "io/loader.h"

class AssetManager;
class Loader;

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

class AssetManager
{
public:
	AssetManager(void);
	AssetManager(const std::string& asset_directory);
	~AssetManager(void);

	HAKSAENGINE_API void set_asset_directory_path(const char* path);
	HAKSAENGINE_API Asset& get_asset(unsigned int asset_id) const;

	template<class AssetType>
	AssetType& get_asset(unsigned int asset_id) const
	{
		return static_cast<AssetType&>(get_asset(asset_id));
	}

	template<class C>
	unsigned int load_asset(const char* asset_name)
	{
		return _load_asset(asset_name, typeid(C));
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
	std::unordered_map<std::type_index, Event> _load_event_map;
	std::unordered_map<unsigned int, Asset*> _assets;

	HAKSAENGINE_API unsigned int _load_asset(const std::string& asset_name, std::type_index type);
};
