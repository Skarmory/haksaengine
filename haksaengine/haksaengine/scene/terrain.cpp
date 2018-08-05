#include "scene/terrain.h"

#include "services.h"

void Terrain::update(unsigned int x, unsigned int y, const TerrainVertex& vertex)
{
	if (x > _width || y > _height)
		return;

	unsigned int flattened_index = _flatten_coord(x, y);

	_vertices[flattened_index] = vertex;

	_update_mesh();
}

const TerrainVertex& Terrain::get_vertex(unsigned int x, unsigned int y) const
{
	unsigned int flattened_index = _flatten_coord(x, y);

	return _vertices[flattened_index];
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
	UpdateBufferObjectCommand* ubo_cmd = new UpdateBufferObjectCommand;
	_tex_data.tileset = _tileset->get_texture()->get_handle();
	ubo_cmd->add_ssbo(new Buffer<TerrainData>(TERRAIN_SSBO_BIND_POINT, &_tex_data, 1));

	_tileset->use();

	_render_cmds.push_back(ubo_cmd);
	_render_cmds.push_back(new BindTerrainMeshCommand(_mesh));
	_render_cmds.push_back(new UseShaderCommand(*_shader));
	_render_cmds.push_back(new DrawCommand(PrimitiveType::Triangles, _indices.size(), 0));

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

void Terrain::_update_mesh(void)
{
	std::vector<TerrainVertex> verts_expanded;
	verts_expanded.reserve(_indices.size());

	unsigned int index;
	for (int i = 0; i < _indices.size(); i += 6)
	{
		index = _indices[i];
		_vertices[index].uv = glm::vec2(0.0f, 1.0f);
		verts_expanded.push_back(_vertices[index]);

		index = _indices[i + 1];
		_vertices[index].uv = glm::vec2(0.0f, 0.0f);
		verts_expanded.push_back(_vertices[index]);

		index = _indices[i + 2];
		_vertices[index].uv = glm::vec2(1.0f, 1.0f);
		verts_expanded.push_back(_vertices[index]);

		index = _indices[i + 3];
		_vertices[index].uv = glm::vec2(1.0f, 1.0f);
		verts_expanded.push_back(_vertices[index]);

		index = _indices[i + 4];
		_vertices[index].uv = glm::vec2(0.0f, 0.0f);
		verts_expanded.push_back(_vertices[index]);

		index = _indices[i + 5];
		_vertices[index].uv = glm::vec2(1.0f, 0.0f);
		verts_expanded.push_back(_vertices[index]);
	}

	_mesh.update(verts_expanded);
}