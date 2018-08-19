#include "scene/terrain.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/norm.hpp>

#include "services.h"

void Terrain::update(TerrainVertexData* data)
{
	unsigned int flattened_index = _flatten_coord(data->x, data->y);

	_vertices[flattened_index] = *data;

	_update_normals(data);

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

void Terrain::_update_normals(TerrainVertexData* data)
{
	if (data->y >= 1)
		_update_normal(get_vertex(data->x, data->y - 1));

	if (data->x >= 1)
		_update_normal(get_vertex(data->x - 1, data->y));

	if (data->x < _width)
		_update_normal(get_vertex(data->x + 1, data->y));

	if (data->y < _height)
		_update_normal(get_vertex(data->x, data->y + 1));
}

void Terrain::_update_normal(TerrainVertexData& data)
{
	float x_diff, z_diff;

	if (data.x >= 1 && data.x < _width)
	{
		x_diff = _vertices[_flatten_coord(data.x - 1, data.y)].position.y - _vertices[_flatten_coord(data.x + 1, data.y)].position.y;
	}
	else if (data.x == 0)
	{
		x_diff = _vertices[_flatten_coord(data.x, data.y)].position.y - _vertices[_flatten_coord(data.x + 1, data.y)].position.y;
	}
	else
	{
		x_diff = _vertices[_flatten_coord(data.x - 1, data.y)].position.y - _vertices[_flatten_coord(data.x, data.y)].position.y;
	}

	if (data.y >= 1 && data.y < _height)
	{
		z_diff = _vertices[_flatten_coord(data.x, data.y - 1)].position.y - _vertices[_flatten_coord(data.x, data.y + 1)].position.y;
	}
	else if (data.y == 0)
	{
		z_diff = _vertices[_flatten_coord(data.x, data.y)].position.y - _vertices[_flatten_coord(data.x, data.y + 1)].position.y;
	}
	else
	{
		z_diff = _vertices[_flatten_coord(data.x, data.y - 1)].position.y - _vertices[_flatten_coord(data.x, data.y)].position.y;
	}

	data.normal = glm::normalize(glm::vec3(x_diff / (float)_tile_size, 2.0f, z_diff / (float)_tile_size));
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
		_quadtree->add(new TerrainTriangle(&_vertices[_indices[i]], &_vertices[_indices[i + 1]], &_vertices[_indices[i + 2]]));
	}
}

TerrainVertexData* Terrain::intersect(const Ray& ray)
{
	glm::vec3 world_pos;

	// Clamp xsection to closest vertex if intersects
	if (intersect(ray, world_pos))
		return &get_vertex(world_pos);

	return nullptr;
}

bool Terrain::intersect(const Ray& ray, glm::vec3& closest)
{
	std::set<TerrainTriangle*> xsects = _quadtree->get_intersections(ray);

	glm::vec3 xsect;
	float closest_length = std::numeric_limits<float>::max();

	// Go through triangles and try intersect
	for(auto tri_ptr : xsects)
	{
		if(tri_ptr->intersect(ray, xsect))
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
		return false;

	// Clamp xsection to closest vertex
	return true;
}

std::set<TerrainTriangle*> Terrain::intersect(Entity* entity)
{
	return _quadtree->get_intersections(entity);
}

const Tileset& Terrain::get_tileset(void)
{
	return *_tileset;
}

void Terrain::save(std::ofstream& os)
{
	os << "Terrain:" << std::endl;
	os << "[" << std::endl;

	os << "\tInfo:" << std::endl;
	os << "\t[" << std::endl;
	os << "\t\tWidth " << _width << std::endl;
	os << "\t\tHeight " << _height << std::endl;
	os << "\t\tTileSize " << _tile_size << std::endl;
	os << "\t\tTileset " << _tileset->get_name() << std::endl;
	os << "\t]" << std::endl;

	os << "\tVertices:" << std::endl;
	os << "\t[" << std::endl;
	for (auto vertex : _vertices)
	{
		os << "\t\t" << vertex.x << " " << vertex.y <<
		      " " << vertex.position.x << "," << vertex.position.y << "," << vertex.position.z <<
			  " " << vertex.normal.x << "," << vertex.normal.y << "," << vertex.normal.z <<
			  " " << vertex.texture << std::endl;
	}
	os << "\t]" << std::endl;

	os << "\tIndices:" << std::endl;
	os << "\t[" << std::endl;
	os << "\t\t";
	int i = 1;
	for (auto index : _indices)
	{
		os << index << " ";
		
		if (i % 32 == 0)
			os << std::endl << "\t\t";

		i++;
	}
	os << "\t]" << std::endl;

	os << "]" << std::endl;
}