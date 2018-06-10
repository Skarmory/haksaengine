#pragma once

#include <string>

#include "globals.h"
#include "io/loader.h"
#include "gfx/mesh.h"

/* Class that can parse and create a mesh object from a file */
class MeshLoader : public Loader<Mesh>
{
public:
	MeshLoader(const std::string& directory);
	Mesh* load(const std::string& id) override;

private:
	void parse_vertices(std::ifstream& stream, Mesh* mesh);
	void parse_indices(std::ifstream& stream, Mesh* mesh);
	void parse_normals(std::ifstream& stream, Mesh* mesh);
};
