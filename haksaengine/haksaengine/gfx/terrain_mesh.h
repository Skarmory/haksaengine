#pragma once

#include <vector>

#include <GL/glew.h>

#include "vertex.h"

class TerrainMesh
{
	friend class Renderer;

public:

	TerrainMesh(void);
	~TerrainMesh(void);

	// No copying
	TerrainMesh(const TerrainMesh&) = delete;
	TerrainMesh& operator=(const TerrainMesh&) = delete;

	// Vertices and indices data accessors
	std::size_t vertex_count(void) const;

	// Methods to load and unload the mesh from GPU memory
	void initialise(void);
	void uninitialise(void);

	// Set the geometry data for the terrain
	void set_data(std::vector<TerrainVertex>&& data);

	// Update a vertex with new data
	void update(unsigned int index, const TerrainVertex& vertex);

	// Return a vertex
	const TerrainVertex& get_vertex(unsigned int index) const;

private:

	bool _initialised;

	GLuint _vertex_array;
	GLuint _vertex_buffer;

	std::vector<TerrainVertex> _vertices;

	// Methods to bind and unbind this mesh
	void _bind(void) const;
	void _unbind(void) const;
};