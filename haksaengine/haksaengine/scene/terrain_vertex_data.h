#pragma once

#include <glm/glm.hpp>

struct TerrainVertexData
{
	unsigned int x;
	unsigned int y;
	glm::vec3 position;
	glm::vec3 normal;
	unsigned int texture;
};