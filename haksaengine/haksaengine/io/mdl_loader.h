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
	// Parse all geoset data and add each geoset to the MDLFile
	void parse_geosets(std::ifstream& stream, MDLFile* mdl);

	// Subparse a geoset, create a Mesh object, and add it to MDLFile
	void parse_geoset(std::ifstream& stream, MDLFile* mdl);

	// Subparse vertex position data and add it to the Mesh object
	void parse_vertices(std::ifstream& stream, Mesh* mesh);

	// Subparse index array data and add it to the Mesh object
	void parse_indices(std::ifstream& stream, Mesh* mesh);

	// Subparse the vertex normals data and add it to the Mesh object
	void parse_normals(std::ifstream& stream, Mesh* mesh);

	// Subparse the vertex texture coordinate data and add it to the Mesh object
	void parse_uvs(std::ifstream& stream, Mesh* mesh);

	// Subparse the vertex bone indices data and add to the Mesh object
	void parse_bone_indices(std::ifstream& stream, Mesh* mesh);

	// Subparse the vertex bone weights and add to the Mesh object
	void parse_bone_weights(std::ifstream& stream, Mesh* mesh);

	// Parse all texture data and add it to the MDLFile. This loads the Texture via the AssetManager
	void parse_textures(std::ifstream& stream, MDLFile* mdl);
};
