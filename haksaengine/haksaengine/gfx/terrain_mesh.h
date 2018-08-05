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

	// Methods to load and unload the mesh from GPU memory
	void initialise(void);
	void uninitialise(void);

	// Update a vertex with new data
	void update(const std::vector<TerrainVertex>& data);

private:

	bool _initialised;
	std::size_t _size;

	GLuint _vertex_array;
	GLuint _vertex_buffer;

	// Methods to bind and unbind this mesh
	void _bind(void) const;
	void _unbind(void) const;
};