#pragma once

#include "globals.h"

#include "scene/terrain.h"

class TerrainGenerator
{
public:

	// Create a default terrain of [width * height] tiles
	HAKSAENGINE_API static Terrain* generate(unsigned int width, unsigned int height);
};