#pragma once

#include <fstream>

#include "globals.h"

#include "scene/terrain.h"

class TerrainGenerator
{
public:

	// Create a default terrain of [width * height] tiles
	HAKSAENGINE_API static Terrain* generate(unsigned int width, unsigned int height, const char* tileset);

	HAKSAENGINE_API static Terrain* generate(std::ifstream& fs);

private:

	static void _parse_info(std::ifstream& fs, Terrain* terrain);

	static void _parse_vertices(std::ifstream& fs, Terrain* terrain);

	static void _parse_indices(std::ifstream& fs, Terrain* terrain);
};