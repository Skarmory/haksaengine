#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include "globals.h"

/* Variant struct contains the argument data of an event. Using a union to represent whatever types we need. */
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

/* Event class for communication between various systems. Events are referenced by a string currently, and carry a vector of arguments */
// TODO: Hashed string ids for better efficiency
// TODO: Replace vector with a staticly sized container
struct Event
{
	std::string event_type;
	std::vector<Variant> arguments;
};