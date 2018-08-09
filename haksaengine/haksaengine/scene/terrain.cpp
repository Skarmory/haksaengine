#include "scene/terrain.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/norm.hpp>

#include "services.h"

void Terrain::update(TerrainVertexData* data)
{
	unsigned int flattened_index = _flatten_coord(data->x, data->y);

	_vertices[flattened_index] = *data;

	_update_mesh();
}

TerrainVertexData& Terrain::get_vertex(unsigned int x, unsigned int y)
{
	unsigned int flattened_index = _flatten_coord(x, y);

	return _vertices[flattened_index];
}

TerrainVertexData& Terrain::get_vertex(const glm::vec3 position)
{
	float half_width = (float)_width * 0.5f;
	float half_height = (float)_height * 0.5f;

	//unsigned int x = std::roundf((position.x / (float)_tile_size) + 0.5f);
	//unsigned int y = std::roundf((position.z / (float)_tile_size) + 0.5f);

	float x = std::roundf((position.x + (half_width * (float)_tile_size)) / (float)_tile_size);
	float y = std::roundf((position.z + (half_height * (float)_tile_size)) / (float)_tile_size);

	return get_vertex(x, y);
}

void Terrain::draw(void)
{
	UpdateBufferObjectCommand* ubo_cmd = new UpdateBufferObjectCommand;
	TerrainData data;

	data.tileset = _tileset->get_texture()->get_handle();

	ubo_cmd->add_uniform(new Buffer<TerrainData>(TERRAIN_SSBO_BIND_POINT, &data, 1));

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
	
	return val * (float)_tile_size;
}

void Terrain::_update_mesh(void)
{
	std::vector<TerrainVertex> verts_expanded;
	verts_expanded.reserve(_indices.size());

	unsigned int face = 0;
	unsigned int idx1, idx2, idx3, idx4, idx5, idx6;

	TerrainVertex tmp;
	for (int i = 0; i < _indices.size(); i += 6)
	{
		idx1 = _indices[i];
		idx2 = _indices[i + 1];
		idx3 = _indices[i + 2];
		idx4 = _indices[i + 3];
		idx5 = _indices[i + 4];
		idx6 = _indices[i + 5];

		tmp.tex[0] = _vertices[idx1].texture;
		tmp.tex[1] = _vertices[idx2].texture;
		tmp.tex[2] = _vertices[idx3].texture;
		tmp.tex[3] = _vertices[idx6].texture;

		tmp.position = _vertices[idx1].position;
		tmp.normal = _vertices[idx1].normal;
		tmp.uv = glm::vec2(0.0f, 1.0f);
		verts_expanded.push_back(tmp);

		tmp.position = _vertices[idx2].position;
		tmp.normal = _vertices[idx2].normal;
		tmp.uv = glm::vec2(0.0f, 0.0f);
		verts_expanded.push_back(tmp);

		tmp.position = _vertices[idx3].position;
		tmp.normal = _vertices[idx3].normal;
		tmp.uv = glm::vec2(1.0f, 1.0f);
		verts_expanded.push_back(tmp);

		tmp.position = _vertices[idx4].position;
		tmp.normal = _vertices[idx4].normal;
		tmp.uv = glm::vec2(1.0f, 1.0f);
		verts_expanded.push_back(tmp);

		tmp.position = _vertices[idx5].position;
		tmp.normal = _vertices[idx5].normal;
		tmp.uv = glm::vec2(0.0f, 0.0f);
		verts_expanded.push_back(tmp);

		tmp.position = _vertices[idx6].position;
		tmp.normal = _vertices[idx6].normal;
		tmp.uv = glm::vec2(1.0f, 0.0f);
		verts_expanded.push_back(tmp);
	}

	_mesh.update(verts_expanded);
}

void Terrain::_update_quadtree(void)
{
	for (unsigned int i = 0; i < _indices.size(); i += 3)
	{
		TerrainTriangle t(&_vertices[_indices[i]], &_vertices[_indices[i + 1]], &_vertices[_indices[i + 2]]);

		_quadtree->add(t);
	}
}

TerrainVertexData* Terrain::intersect(const Ray& ray)
{
	TerrainVertexData *v1, *v2, *v3;

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

const Tileset& Terrain::get_tileset(void)
{
	return *_tileset;
}