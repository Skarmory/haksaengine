#pragma once

#include <glm/glm.hpp>

#include "gfx/terrain_mesh.h"
#include "gfx/tileset.h"
#include "gfx/shader.h"
#include "gfx/render_command.h"
#include "gfx/uniform_data.h"

#include "scene/ray.h"
#include "scene/quadtree.h"

class TerrainQuadTree;

struct TerrainVertexData
{
	unsigned int x;
	unsigned int y;
	glm::vec3 position;
	glm::vec3 normal;
	unsigned int texture;
};

class Terrain
{
	friend class TerrainGenerator;

public:

	// Update data of a terrain mesh vertex
	HAKSAENGINE_API void update(TerrainVertexData* data);

	// Get a terrain vertex at given x, y index
	HAKSAENGINE_API TerrainVertexData& get_vertex(unsigned int x, unsigned int y);

	// Get a terrain vertex by clamping a world position to closest vertex
	HAKSAENGINE_API TerrainVertexData& get_vertex(const glm::vec3 position);

	// Generate draw commands for this piece of terrain
	void draw(void);

	// Get the vertex that we intersect
	HAKSAENGINE_API TerrainVertexData* intersect(const Ray& ray);

	// Gets const reference to the tileset the terrain uses
	HAKSAENGINE_API const Tileset& get_tileset(void);

private:

	unsigned int _flatten_coord(unsigned int x, unsigned int y) const;
	glm::vec2    _unflatten_coord(unsigned int index) const;

	glm::vec3 _index_to_world(unsigned int x, unsigned int y) const;

	void _update_mesh(void);
	void _update_quadtree(void);

	unsigned int _tile_size;
	unsigned int _width;
	unsigned int _height;

	TerrainMesh _mesh;
	TerrainQuadTree* _quadtree;

	Tileset* _tileset;
	const Shader* _shader;

	std::vector<TerrainVertexData> _vertices;
	std::vector<unsigned int> _indices;

	std::vector<const RenderCommand*> _render_cmds;
};