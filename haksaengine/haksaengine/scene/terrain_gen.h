#pragma once

#include <glm/glm.hpp>

#include "terrain.h"

class TerrainGenerator
{
public:

	static Terrain* generate(unsigned int width, unsigned int height, glm::vec3 centre);
};