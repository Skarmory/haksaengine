#include "scene/terrain.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/norm.hpp>

#include "services.h"

void Terrain::update(unsigned int x, unsigned int y, const TerrainVertex& vertex)
{
	if (x > _width || y > _height)
		return;

	unsigned int flattened_index = _flatten_coord(x, y);

	_vertices[flattened_index] = vertex;

	_update_mesh();
}

TerrainVertex& Terrain::get_vertex(unsigned int x, unsigned int y)
{
	unsigned int flattened_index = _flatten_coord(x, y);

	return _vertices[flattened_index];
}

TerrainVertex& Terrain::get_vertex(const glm::vec3 position)
{
	unsigned int x = std::roundf((position.x / (float)_tile_size) + 0.5f);
	unsigned int y = std::roundf((position.z / (float)_tile_size) + 0.5f);

	return get_vertex(x, y);
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

	unsigned int face = 0;
	unsigned int tile;
	unsigned int t1, t2, t3, t4;
	unsigned int index;
	for (int i = 0; i < _indices.size(); i += 6)
	{
		index = _indices[i];
		t1 = _vertex_texture[index];
		_vertices[index].uv = glm::vec2(0.0f, 1.0f);
		_vertices[index].face_idx = face;
		verts_expanded.push_back(_vertices[index]);

		index = _indices[i + 1];
		t2 = _vertex_texture[index];
		_vertices[index].uv = glm::vec2(0.0f, 0.0f);
		_vertices[index].face_idx = face;
		verts_expanded.push_back(_vertices[index]);

		index = _indices[i + 2];
		t3 = _vertex_texture[index];
		_vertices[index].uv = glm::vec2(1.0f, 1.0f);
		_vertices[index].face_idx = face;
		verts_expanded.push_back(_vertices[index]);

		face++;

		index = _indices[i + 3];
		_vertices[index].uv = glm::vec2(1.0f, 1.0f);
		_vertices[index].face_idx = face;
		verts_expanded.push_back(_vertices[index]);

		index = _indices[i + 4];
		_vertices[index].uv = glm::vec2(0.0f, 0.0f);
		_vertices[index].face_idx = face;
		verts_expanded.push_back(_vertices[index]);

		index = _indices[i + 5];
		t4 = _vertex_texture[index];
		_vertices[index].uv = glm::vec2(1.0f, 0.0f);
		_vertices[index].face_idx = face;
		verts_expanded.push_back(_vertices[index]);

		_tex_data.textures[face / 2] = { t1,t2,t3,t4 };

		face++;
	}

	_mesh.update(verts_expanded);
}

TerrainVertex* Terrain::intersect(const Ray& ray)
{
	TerrainVertex *v1, *v2, *v3;

	glm::vec3 closest;
	glm::vec3 xsect;
	float closest_length = std::numeric_limits<float>::max();

	unsigned int index1, index2, index3;

	// Go through triangles and try intersect
	for (unsigned int i = 0; i < _indices.size(); i += 3)
	{
		index1 = _indices[i];
		index2 = _indices[i + 1];
		index3 = _indices[i + 2];

		v1 = &_vertices[index1];
		v2 = &_vertices[index2];
		v3 = &_vertices[index3];

		if (intersect_triangle(ray, v1->position, v2->position, v3->position, xsect))
		{
			// We intersect here, now check if it's closer
			if (glm::length2(xsect) < closest_length)
			{
				closest_length = glm::length2(xsect - ray.position);
				closest = xsect;
			}
		}
	}

	// No intersection at all
	if (closest_length == std::numeric_limits<float>::max())
		return nullptr;

	// Clamp xsection to closest vertex
	return &get_vertex(xsect);
}