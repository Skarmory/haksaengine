#include "gfx/terrain_mesh.h"

TerrainMesh::TerrainMesh(void) : _initialised(false), _size(0), _vertex_array(0), _vertex_buffer(0)
{
}

TerrainMesh::~TerrainMesh(void)
{
	if (_initialised)
		uninitialise();
}

void TerrainMesh::initialise(void)
{
	if (_initialised)
		return;

	glCreateVertexArrays(1, &_vertex_array);
	glCreateBuffers(1, &_vertex_buffer);

	glBindVertexArray(_vertex_array);

		glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (GLvoid*)offsetof(TerrainVertex, position));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (GLvoid*)offsetof(TerrainVertex, normal));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (GLvoid*)offsetof(TerrainVertex, uv));
		glEnableVertexAttribArray(2);

		glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, sizeof(TerrainVertex), (GLvoid*)offsetof(TerrainVertex, face_idx));
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
}

void TerrainMesh::_unbind(void) const
{
	if (!_initialised)
		return;

	glBindVertexArray(0);
}

void TerrainMesh::update(const std::vector<TerrainVertex>& data)
{
	std::size_t size_bytes = data.size() * sizeof(TerrainVertex);

	if (size_bytes > _size)
	{
		glBufferData(GL_ARRAY_BUFFER, size_bytes, data.data(), GL_STATIC_DRAW);
		_size = size_bytes;
	}
	else
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, size_bytes, data.data());
	}
}