#pragma once

#include <string>

#include "globals.h"

#include "asset_manager.h"

class Asset;

// Base class that all loaders need to inherit from
class Loader
{
public:
	// Construct new loader that loads from a given directory subfolder of the asset path, and handling a particular file extension
	Loader(const std::string& directory, const std::string& extension) : _directory(directory), _extension(extension)
	{
	}

	virtual ~Loader(void)
	{
	}

	virtual Asset* load(const std::string& name) = 0;

protected:
	std::string _asset_path;
	std::string _directory;
	std::string _extension;

	// Constructs full filepath for a given file
	std::string get_path(const std::string& asset_name)
	{
		auto ext_idx = asset_name.find('.');
		if (ext_idx == std::string::npos)
		{
			return _asset_path + _directory + asset_name + _extension;
		}
		else
		{
			return _asset_path + _directory + asset_name.substr(0, ext_idx) + _extension;
		}
	}

	friend class AssetManager;
};