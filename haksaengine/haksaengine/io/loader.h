#pragma once

#include <string>

#include "globals.h"

// Allows Loaders to be added to collections
class BaseLoader
{
};

// Templated loader base
template<class C>
class Loader : public BaseLoader
{
public:
	Loader(const std::string& directory, const std::string& extension) : _directory(directory), _extension(extension)
	{
	}

	virtual C* load(const std::string& name) = 0;

protected:
	std::string _directory;
	std::string _extension;
};