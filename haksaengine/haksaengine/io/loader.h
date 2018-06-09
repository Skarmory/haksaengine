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
	Loader(const char* directory) : _directory(directory)
	{
	}

	virtual C* load(const char* name) = 0;

private:
	const char* _directory;
};