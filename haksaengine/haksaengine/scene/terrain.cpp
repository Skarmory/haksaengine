#include "scene/terrain.h"

void Terrain::update(unsigned int x, unsigned int y, const TerrainVertex& vertex)
{
	if (x > _width || y > _height)
		return;

	unsigned int flattened_index = _flatten_coord(x, y);

	_mesh.update(flattened_index, vertex);
}

const TerrainVertex& Terrain::get_vertex(unsigned int x, unsigned int y) const
{
	unsigned int flattened_index = _flatten_coord(x, y);

	return _mesh.get_vertex(flattened_index);
}

const TerrainVertex& Terrain::get_vertex(const glm::vec3 position) const
{
	// Convert world space position to indices
	float half_width = (float)_width * 0.5f;
	float half_height = (float)_height * 0.5f;

	glm::vec2 tmp;
	tmp.x = position.x + half_width + 0.5f;
	tmp.y = position.y + half_height + 0.5f;

	return get_vertex(tmp.x, tmp.y);
}

unsigned int Terrain::_flatten_coord(unsigned int x, unsigned int y) const
{
	return ((_width + 1) * y) + x;
}