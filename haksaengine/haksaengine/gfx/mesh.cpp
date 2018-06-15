#include "gfx/mesh.h"

Mesh::Mesh(void) : initialised(false), vertex_array(0), vertex_buffer(0), index_buffer(0)
{
}

Mesh::~Mesh(void)
{
	if (initialised)
		uninitialise();
}

// Generate vertex array object, vertex buffer, and index buffer
void Mesh::initialise(void)
{
	glGenVertexArrays(1, &vertex_array);
	glGenBuffers(1, &vertex_buffer);
	glGenBuffers(1, &index_buffer);

	glBindVertexArray(vertex_array);
		
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(1);
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
		//glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	initialised = true;
}

void Mesh::uninitialise(void)
{
	glDeleteBuffers(2, &vertex_buffer);
	glDeleteVertexArrays(1, &vertex_array);

	vertex_array = 0;
	vertex_buffer = 0;
	index_buffer = 0;

	initialised = false;
}

// Wrapper to bind vertex array
void Mesh::bind(void) const
{
	glBindVertexArray(vertex_array);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
}


// Wrapper to unbind vertex array
void Mesh::unbind(void) const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}