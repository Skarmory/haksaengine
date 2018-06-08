#pragma once

#include "globals.h"

/* The component class contains the data that is attached to an entity. */
/* This should be derived from for each logical grouping of data. */

class Component
{
public:
	HAKSAENGINE_API Component(void) = default;
	HAKSAENGINE_API virtual ~Component(void) = default;
};
