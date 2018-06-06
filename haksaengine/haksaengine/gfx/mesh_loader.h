#pragma once

#include <string>

#include "globals.h"
#include "mesh.h"

/* Class that can parse and create a mesh object from a file */
class HAKSAENGINE_API MeshLoader
{
public:
	Mesh* load_mesh(const std::string& filename);

private:
	void parse_vertices(std::ifstream& stream, Mesh* mesh);
	void parse_indices(std::ifstream& stream, Mesh* mesh);
	void parse_normals(std::ifstream& stream, Mesh* mesh);
};
