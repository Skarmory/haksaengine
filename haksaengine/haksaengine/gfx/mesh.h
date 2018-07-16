#pragma once

#include <vector>

#include "GL/glew.h"
#include "vertex.h"
#include "asset_manager.h"

class MDLLoader;
class Renderer;

// Contains geometry information and wraps OpenGL objects
struct Mesh
{
	Mesh(void);
	~Mesh(void);

	// No copying
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	// Vertices and indices data accessors
	int vertex_count(void) const;
	int index_count(void) const;

private:
	bool _initialised;

	GLuint _vertex_array;
	GLuint _vertex_buffer;
	GLuint _index_buffer;

	std::vector<Vertex> _vertices;
	std::vector<unsigned int> _indices;

	// Methods to load and unload the mesh from GPU memory
	void _initialise(void);
	void _uninitialise(void);

	// Methods to bind and unbind this mesh
	void _bind(void) const;
	void _unbind(void) const;

	friend class MDLLoader;
	friend class Renderer;
};