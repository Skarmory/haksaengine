#include "scene/terrain.h"

#include "services.h"

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

void Terrain::draw(void)
{
	_render_cmds.push_back(new BindTerrainMeshCommand(_mesh));
	_render_cmds.push_back(new UseShaderCommand(*_shader));
	_render_cmds.push_back(new DrawIndexedCommand(PrimitiveType::Triangles, _mesh.index_count(), 0));

	Services::get().get_renderer()->submit_render_commands(_render_cmds);
}

unsigned int Terrain::_flatten_coord(unsigned int x, unsigned int y) const
{
	return ((_width + 1) * y) + x;
}

glm::vec2 Terrain::_unflatten_coord(unsigned int index) const
{
	glm::vec2 val;

	val.x = (index % (_width + 1));
	val.y = (index / (_width + 1));

	return val;
}

glm::vec3 Terrain::_index_to_world(unsigned int x, unsigned int y) const
{
	float half_width = (float)_width * 0.5f;
	float half_height = (float)_height * 0.5f;

	glm::vec3 val = glm::vec3(0.0f);

	val.x = x - half_width;
	/*val.y = y - half_height;*/
	val.z = y - half_height;
	
	return val;
}