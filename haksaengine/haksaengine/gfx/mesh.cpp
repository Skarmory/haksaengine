#include "gfx/mesh.h"

Mesh::Mesh(void) : _initialised(false), _vertex_array(0), _vertex_buffer(0), _index_buffer(0)
{
}

Mesh::~Mesh(void)
{
	if (_initialised)
		_uninitialise();
}

// Generate vertex array object, vertex buffer, and index buffer
void Mesh::_initialise(void)
{
	if (_initialised)
		return;

	glGenVertexArrays(1, &_vertex_array);
	glGenBuffers(1, &_vertex_buffer);
	glGenBuffers(1, &_index_buffer);

	glBindVertexArray(_vertex_array);
		
		glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), _indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
		glEnableVertexAttribArray(2);

		glVertexAttribIPointer(3, 4, GL_UNSIGNED_INT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, bone_ids));
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, bone_weights));
		glEnableVertexAttribArray(4);

	glBindVertexArray(0);

	_initialised = true;
}

void Mesh::_uninitialise(void)
{
	if (!_initialised)
		return;

	glDeleteBuffers(2, &_vertex_buffer);
	glDeleteVertexArrays(1, &_vertex_array);

	_vertex_array = 0;
	_vertex_buffer = 0;
	_index_buffer = 0;

	_initialised = false;
}

// Wrapper to bind vertex array
void Mesh::_bind(void) const
{
	if (!_initialised)
		return;

	glBindVertexArray(_vertex_array);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer);
}


// Wrapper to unbind vertex array
void Mesh::_unbind(void) const
{
	if (!_initialised)
		return;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

int Mesh::vertex_count(void) const
{
	return _vertices.size();
}

int Mesh::index_count(void) const
{
	return _indices.size();
}