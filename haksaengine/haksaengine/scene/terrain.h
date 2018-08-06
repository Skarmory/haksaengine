#pragma once

#include <glm/glm.hpp>

#include "gfx/terrain_mesh.h"
#include "gfx/tileset.h"
#include "gfx/shader.h"
#include "gfx/render_command.h"
#include "gfx/uniform_data.h"

#include "scene/ray.h"

class Terrain
{
	friend class TerrainGenerator;

public:

	// Update data of a terrain mesh vertex
	void update(unsigned int x, unsigned int y, const TerrainVertex& vertex);

	// Get a terrain vertex at given x, y index
	TerrainVertex& get_vertex(unsigned int x, unsigned int y);

	// Get a terrain vertex by clamping a world position to closest vertex
	TerrainVertex& get_vertex(const glm::vec3 position);

	// Generate draw commands for this piece of terrain
	void draw(void);

	TerrainVertex* intersect(const Ray& ray);

private:

	unsigned int _flatten_coord(unsigned int x, unsigned int y) const;
	glm::vec2    _unflatten_coord(unsigned int index) const;

	glm::vec3 _index_to_world(unsigned int x, unsigned int y) const;

	void _update_mesh(void);

	unsigned int _tile_size;
	unsigned int _width;
	unsigned int _height;

	TerrainMesh _mesh;
	TerrainData _tex_data;

	Tileset* _tileset;
	const Shader* _shader;

	std::vector<TerrainVertex> _vertices;
	std::vector<unsigned int> _indices;
	std::vector<unsigned int> _vertex_texture;

	std::vector<const RenderCommand*> _render_cmds;
};