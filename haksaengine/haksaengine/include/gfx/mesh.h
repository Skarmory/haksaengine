#pragma once

#include <vector>

#include "vertex.h"

struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};