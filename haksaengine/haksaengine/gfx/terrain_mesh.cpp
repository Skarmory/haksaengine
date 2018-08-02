#include "gfx/terrain_mesh.h"

TerrainMesh::TerrainMesh(void) : _initialised(false), _vertex_array(0), _vertex_buffer(0), _index_buffer(0)
{
}

TerrainMesh::~TerrainMesh(void)
{
	if (_initialised)
		uninitialise();
}

std::size_t TerrainMesh::vertex_count(void) const
{
	return _vertices.size();
}

std::size_t TerrainMesh::index_count(void) const
{
	return _indices.size();
}

void TerrainMesh::initialise(void)
{
	if (_initialised)
		return;

	glCreateVertexArrays(1, &_vertex_array);
	glCreateBuffers(2, &_vertex_buffer);

	glBindVertexArray(_vertex_array);

		glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(TerrainVertex), _vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), _indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (GLvoid*)offsetof(TerrainVertex, position));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (GLvoid*)offsetof(TerrainVertex, normal));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (GLvoid*)offsetof(TerrainVertex, uv));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(TerrainVertex), (GLvoid*)offsetof(TerrainVertex, texture_index));
		glEnableVertexAttribArray(3);

	glBindVertexArray(0);

	_initialised = true;
}

void TerrainMesh::uninitialise(void)
{
	if (!_initialised)
		return;

	glDeleteBuffers(1, &_vertex_buffer);
	glDeleteVertexArrays(1, &_vertex_array);

	_vertex_array = 0;
	_vertex_buffer = 0;

	_initialised = false;
}

void TerrainMesh::_bind(void) const
{
	if (!_initialised)
		return;

	glBindVertexArray(_vertex_array);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer);
}

void TerrainMesh::_unbind(void) const
{
	if (!_initialised)
		return;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TerrainMesh::set_data(std::vector<TerrainVertex>&& vertices, std::vector<unsigned int>&& indices)
{
	_vertices = vertices;
	_indices = indices;
}

void TerrainMesh::update(unsigned int index, const TerrainVertex& vertex)
{
	_vertices[index] = vertex;

	glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(TerrainVertex), sizeof(TerrainVertex), &vertex);
}

const TerrainVertex& TerrainMesh::get_vertex(unsigned int index) const
{
	return _vertices[index];
}