#pragma once

#include <glm/glm.hpp>

#include "gfx/terrain_mesh.h"
#include "gfx/tileset.h"
#include "gfx/shader.h"
#include "gfx/render_command.h"
#include "gfx/uniform_data.h"

class Terrain
{
	friend class TerrainGenerator;

public:

	// Update data of a terrain mesh vertex
	void update(unsigned int x, unsigned int y, const TerrainVertex& vertex);

	// Get a terrain vertex at given x, y index
	const TerrainVertex& get_vertex(unsigned int x, unsigned int y) const;

	// Get a terrain vertex by clamping a world position to closest vertex
	const TerrainVertex& get_vertex(const glm::vec3 position) const;

	// Generate draw commands for this piece of terrain
	void draw(void);

private:

	unsigned int _flatten_coord(unsigned int x, unsigned int y) const;
	glm::vec2    _unflatten_coord(unsigned int index) const;

	glm::vec3 _index_to_world(unsigned int x, unsigned int y) const;

	void _update_mesh(void);

	unsigned int _width;
	unsigned int _height;
	TerrainMesh _mesh;
	Tileset* _tileset;
	const Shader* _shader;
	TerrainData _tex_data;

	std::vector<TerrainVertex> _vertices;
	std::vector<unsigned int> _indices;
	std::vector<unsigned int> _vertex_texture;

	std::vector<const RenderCommand*> _render_cmds;
};