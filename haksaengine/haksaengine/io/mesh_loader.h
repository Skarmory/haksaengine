#pragma once

#include <string>
#include <fstream>

#include "globals.h"
#include "io/loader.h"
#include "gfx/mesh.h"
#include "io/mdl.h"

/* Class that can parse and create a mesh object from a file */
class MDLLoader : public Loader
{
public:
	MDLLoader(const std::string& directory);
	MDLFile* load(const std::string& id) override;

private:
	void parse_geosets(std::ifstream& stream, MDLFile* mdl);
	void parse_geoset(std::ifstream& stream, MDLFile* mdl);
	void parse_vertices(std::ifstream& stream, Mesh* mesh);
	void parse_indices(std::ifstream& stream, Mesh* mesh);
	void parse_normals(std::ifstream& stream, Mesh* mesh);
	void parse_uvs(std::ifstream& stream, Mesh* mesh);
	void parse_texture(std::ifstream& stream, MDLFile* mdl);
	void parse_transform(std::ifstream& stream, Mesh* mesh);
};
