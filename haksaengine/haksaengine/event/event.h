#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include "globals.h"
#include "variant.h"

struct Variant;

/* Event class for communication between various systems. Events are referenced by a string currently, and carry a vector of arguments */
// TODO: Hashed string ids for better efficiency
// TODO: Replace vector with a staticly sized container
struct Event
{
	std::string event_type;
	std::vector<Variant> arguments;
};