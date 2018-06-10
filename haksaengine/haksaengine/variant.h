#pragma once

#include <unordered_map>

#include "globals.h"

/* Variant struct contains variable data. Using a union to represent whatever types we need. */
struct Variant
{
	enum Type
	{
		INTEGER,
		UNSIGNEDINT,
		FLOAT,
		BOOL,
		COUNT
	};

	Type type;

	union
	{
		int          as_int;
		unsigned int as_uint;
		float        as_float;
		bool         as_bool;
	};
};

class NamedVariantPack
{
public:
	HAKSAENGINE_API void set(const char* name, Variant data);
	HAKSAENGINE_API Variant get(const char* name);

private:
	std::unordered_map<std::string, Variant> _data_map;
};