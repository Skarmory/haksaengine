#pragma once

#include <vector>

#include "GL/glew.h"
#include "vertex.h"

struct Mesh
{
	std::string name;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	void initialise(void);
	void bind(void);
	void unbind(void);

private:
	GLuint vertex_array, vertex_buffer, index_buffer;
};