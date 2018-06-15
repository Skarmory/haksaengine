#pragma once

#include <vector>

#include "GL/glew.h"
#include "vertex.h"
#include "asset_manager.h"

class Asset;
class MeshLoader;
class Renderer;

struct Mesh : public Asset
{
	Mesh(void);
	~Mesh(void);

	// No copying
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	// Methods to load and unload the mesh from GPU memory
	void initialise(void);
	void uninitialise(void);

	// Methods to bind and unbind this mesh
	void bind(void) const;
	void unbind(void) const;

private:
	bool initialised;

	GLuint vertex_array, vertex_buffer, index_buffer;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	friend class MeshLoader;
	friend class Renderer;
};