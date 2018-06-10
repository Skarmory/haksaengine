#pragma once

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