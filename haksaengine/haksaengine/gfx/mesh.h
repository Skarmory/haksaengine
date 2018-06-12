#pragma once

#include <vector>

#include "GL/glew.h"
#include "vertex.h"
#include "asset_manager.h"

class Asset;

struct Mesh : public Asset
{
	Mesh(void);
	~Mesh(void);

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	std::string name;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	void initialise(void);
	void bind(void);
	void unbind(void);

private:
	GLuint vertex_array, vertex_buffer, index_buffer;
};