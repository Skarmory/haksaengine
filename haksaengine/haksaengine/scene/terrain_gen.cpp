#include "scene/terrain_gen.h"

#include <glm/glm.hpp>

#include "services.h"

Terrain* TerrainGenerator::generate(unsigned int width, unsigned int height, const char* tileset)
{
	Terrain* terrain = new Terrain;
	terrain->_width = width;
	terrain->_height = height;
	terrain->_tile_size = 128;

	std::vector<TerrainVertexData> vertices;
	std::vector<unsigned int> indices;
	vertices.reserve(width * height);
	indices.reserve(width * height * 6);

	TerrainVertexData vertex;

	// UV coordinates tile in x and y, and thus tessellate together nicely
	// This allows us to use an index buffer and not split any vertices

	// Quick lookup array for texture coordinates based on map x,y
	glm::vec2 uvs[2][2] = {
		{ glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f) }
	};

	// Create mesh data
	for (unsigned int y = 0; y <= height; y++)
	for (unsigned int x = 0; x <= width; x++)
	{
		// Create vertex
		vertex.x = x;
		vertex.y = y;
		vertex.position = terrain->_index_to_world(x, y);
		vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
		vertex.texture = 0;

		vertices.push_back(vertex);

		// Construct triangles
		if (x != width && y != height)
		{
			// Upper
			indices.push_back(terrain->_flatten_coord(x, y));
			indices.push_back(terrain->_flatten_coord(x, y + 1));
			indices.push_back(terrain->_flatten_coord(x + 1, y));
			

			// Lower
			indices.push_back(terrain->_flatten_coord(x + 1, y));
			indices.push_back(terrain->_flatten_coord(x, y + 1));
			indices.push_back(terrain->_flatten_coord(x + 1, y + 1));
		}

		//terrain->_vertex_texture.push_back(0);
	}

	glm::vec3 min = terrain->_index_to_world(0, 0);
	glm::vec3 max = terrain->_index_to_world(width, height);

	min.y = -100.0f;
	max.y = 100.0f;

	terrain->_vertices = vertices;
	terrain->_indices = indices;
	terrain->_quadtree = new TerrainQuadTree(AABB(min, max));
	terrain->_update_quadtree();
	terrain->_mesh.initialise();
	terrain->_update_mesh();
	terrain->_shader = &Services::get().get_asset_manager()->load_and_get_asset<Shader>("terrain.shader");
	terrain->_tileset = &Services::get().get_asset_manager()->load_and_get_asset<Tileset>(tileset);

	return terrain;
}

Terrain* TerrainGenerator::generate(std::ifstream& fs)
{
	Terrain* terrain = new Terrain;

	std::string line;
	std::string data;

	while (std::getline(fs, line))
	{
		if (line.find('[') != std::string::npos)
			continue;

		if (line.find(']') != std::string::npos)
			break;

		auto idx1 = line.find_first_not_of('\t');
		auto idx2 = line.find_first_of(':');

		data = line.substr(idx1, idx2 - idx1);

		if (data == "Info")
		{
			_parse_info(fs, terrain);
		}
		else if (data == "Vertices")
		{
			_parse_vertices(fs, terrain);
		}
		else if (data == "Indices")
		{
			_parse_indices(fs, terrain);
		}
	}

	glm::vec3 min = terrain->_index_to_world(0, 0);
	glm::vec3 max = terrain->_index_to_world(terrain->_width, terrain->_height);

	min.y = -25.0f;
	max.y = 25.0f;

	terrain->_quadtree = new TerrainQuadTree(AABB(min, max));
	terrain->_update_quadtree();
	terrain->_mesh.initialise();
	terrain->_update_mesh();
	terrain->_shader = &Services::get().get_asset_manager()->load_and_get_asset<Shader>("terrain.shader");

	return terrain;
}

void TerrainGenerator::_parse_info(std::ifstream& fs, Terrain* terrain)
{
	std::string line;
	std::string data;

	while (std::getline(fs, line))
	{
		if (line.find('[') != std::string::npos)
			continue;

		if (line.find(']') != std::string::npos)
			break;

		auto idx1 = line.find_first_not_of('\t');
		auto idx2 = line.find_first_of(' ');

		data = line.substr(idx1, idx2 - idx1);

		if (data == "Width")
		{
			idx1 = line.find_first_not_of(' ', idx2);

			data = line.substr(idx1, line.size() - idx1);

			terrain->_width = std::stoul(data);
		}
		else if (data == "Height")
		{
			idx1 = line.find_first_not_of(' ', idx2);

			data = line.substr(idx1, line.size() - idx1);

			terrain->_height = std::stoul(data);
		}
		else if (data == "TileSize")
		{
			idx1 = line.find_first_not_of(' ', idx2);

			data = line.substr(idx1, line.size() - idx1);

			terrain->_tile_size = std::stoul(data);
		}
		else if (data == "Tileset")
		{
			idx1 = line.find_first_not_of(' ', idx2);

			data = line.substr(idx1, line.size() - idx1);

			terrain->_tileset = &Services::get().get_asset_manager()->load_and_get_asset<Tileset>(data.c_str());
		}
	}

	terrain->_vertices.reserve(terrain->_width * terrain->_height);
	terrain->_indices.reserve(terrain->_width * terrain->_height * 6);
}

void TerrainGenerator::_parse_vertices(std::ifstream& fs, Terrain* terrain)
{
	std::string line;
	std::string data;

	while (std::getline(fs, line))
	{
		if (line.find('[') != std::string::npos)
			continue;

		if (line.find(']') != std::string::npos)
			break;

		TerrainVertexData vdata;

		// X
		auto idx1 = line.find_first_not_of('\t');
		auto idx2 = line.find_first_of(' ');
		data = line.substr(idx1, idx2 - idx1);
		vdata.x = std::stoul(data);

		// Y
		idx1 = line.find_first_not_of(' ', idx2);
		idx2 = line.find_first_of(' ', idx1);
		data = line.substr(idx1, idx2 - idx1);
		vdata.y = std::stoul(data);

		// Position x
		idx1 = line.find_first_not_of(' ', idx2);
		idx2 = line.find_first_of(',', idx1);
		data = line.substr(idx1, idx2 - idx1);
		vdata.position.x = std::stof(data);

		// Position y
		idx1 = line.find_first_not_of(',', idx2);
		idx2 = line.find_first_of(',', idx1);
		data = line.substr(idx1, idx2 - idx1);
		vdata.position.y = std::stof(data);

		// Position z
		idx1 = line.find_first_not_of(',', idx2);
		idx2 = line.find_first_of(' ', idx1);
		data = line.substr(idx1, idx2 - idx1);
		vdata.position.z = std::stof(data);

		// Normal x
		idx1 = line.find_first_not_of(' ', idx2);
		idx2 = line.find_first_of(',', idx1);
		data = line.substr(idx1, idx2 - idx1);
		vdata.normal.x = std::stof(data);

		// Normal y
		idx1 = line.find_first_not_of(',', idx2);
		idx2 = line.find_first_of(',', idx1);
		data = line.substr(idx1, idx2 - idx1);
		vdata.normal.y = std::stof(data);

		// Normal z
		idx1 = line.find_first_not_of(',', idx2);
		idx2 = line.find_first_of(' ', idx1);
		data = line.substr(idx1, idx2 - idx1);
		vdata.normal.z = std::stof(data);

		// Texture id
		idx1 = line.find_first_not_of(' ', idx2);
		data = line.substr(idx1, line.size() - idx1);
		vdata.texture = std::stoul(data);

		terrain->_vertices.push_back(vdata);
	}
}

void TerrainGenerator::_parse_indices(std::ifstream& fs, Terrain* terrain)
{
	std::string line;
	std::string data;

	while (std::getline(fs, line))
	{
		if (line.find('[') != std::string::npos)
			continue;

		if (line.find(']') != std::string::npos)
			break;

		TerrainVertexData vdata;

		// First one has tabs
		auto idx1 = line.find_first_not_of('\t');
		auto idx2 = line.find_first_of(' ');
		data = line.substr(idx1, idx2 - idx1);
		terrain->_indices.push_back(std::stoul(data));

		// Rest have spaces
		for (int i = 1; i < 32; i++)
		{
			idx1 = line.find_first_not_of(' ', idx2);
			idx2 = line.find_first_of(' ', idx1);

			if (idx1 == std::string::npos)
				break;

			data = line.substr(idx1, idx2 - idx1);
			terrain->_indices.push_back(std::stoul(data));
		}
	}
}