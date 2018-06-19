#include "io/mesh_loader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "services.h"

MDLLoader::MDLLoader(const std::string& directory) : Loader(directory, ".mdl")
{
}

MDLFile* MDLLoader::load(const std::string& id)
{
	std::string file_path = get_path(id);

	MDLFile* mdl = new MDLFile;
	std::ifstream fs;
	std::string line;

	fs.open(file_path);

	std::vector<unsigned int> texture_ids, mesh_ids;

	while (std::getline(fs, line))
	{
		if (line.find_first_not_of(' ') == std::string::npos)
			continue;

		auto idx = line.find_first_of(' ');
		auto value = line.substr(0, idx);

		if (value == "Textures")
		{
			idx = line.find_first_not_of(' ', idx);
			auto idx1 = line.find_first_of(' ', idx);

			value = line.substr(idx, idx1 - idx);
			mdl->_textures.reserve(std::stoi(value));

			parse_texture(fs, mdl);
		}
		else if (value == "Geosets")
		{
			idx = line.find_first_not_of(' ', idx);
			auto idx1 = line.find_first_of(' ', idx);

			value = line.substr(idx, idx1 - idx);
			mdl->_meshes.reserve(std::stoi(value));
			mdl->_data.reserve(std::stoi(value));

			parse_geosets(fs, mdl);
		}
	}

	fs.close();

	for(auto mesh : mdl->_meshes)
		mesh->initialise();

	return mdl;
}

void MDLLoader::parse_geosets(std::ifstream& stream, MDLFile* mdl)
{
	std::string line, value;

	while (std::getline(stream, line))
	{
		if (line.find_first_not_of(' ') == std::string::npos)
			continue;

		auto idx1 = line.find_first_not_of('\t', 0);
		auto idx2 = line.find_first_of(' ');

		value = line.substr(idx1, idx2 -idx1);

		if (value == "Geoset")
		{
			parse_geoset(stream, mdl);
		}
	}
}

void MDLLoader::parse_geoset(std::ifstream& stream, MDLFile* mdl)
{
	std::string line, value;
	Mesh* mesh = new Mesh;
	MDLData data;

	while (std::getline(stream, line))
	{
		if (line.find_first_not_of(' ') == std::string::npos)
			break;

		auto idx1 = line.find_first_not_of('\t', 0);
		auto idx2 = line.find_first_of(' ');

		value = line.substr(idx1, idx2 - idx1);

		if (value == "Vertices")
		{
			idx1 = line.find_first_not_of(' ', idx2);
			idx2 = line.find_first_of(' ', idx1);

			value = line.substr(idx1, idx2 - idx1);
			mesh->vertices.resize(std::stoi(value));
			parse_vertices(stream, mesh);
		}
		else if (value == "Indices")
		{
			idx1 = line.find_first_not_of(' ', idx2);
			idx2 = line.find_first_of(' ', idx1);

			value = line.substr(idx1, idx2 - idx1);
			mesh->indices.resize(std::stoi(value));
			parse_indices(stream, mesh);
		}
		else if (value == "Normals")
		{
			parse_normals(stream, mesh);
		}
		else if (value == "UVs")
		{
			parse_uvs(stream, mesh);
		}
		else if (value == "Texture")
		{
			idx2 = line.find_first_of(' ', idx1);

			value = line.substr(idx2, line.size() - idx2);
			data.texture_id = std::stoul(value);
		}
		else if (value == "Transform")
		{
			parse_transform(stream, mesh);
		}
	}

	data.mesh_id = mdl->_data.size();

	mdl->_meshes.push_back(mesh);
	mdl->_data.push_back(data);
}

void MDLLoader::parse_vertices(std::ifstream& stream, Mesh* mesh)
{
	std::string line, substr;

	unsigned int vidx = 0;
	while(vidx < mesh->vertices.size())
	{
		std::getline(stream, line);

		auto idx = line.find_first_of(' ', 0);
		auto idx1 = line.find_first_of(',', idx);
		substr = line.substr(idx, idx1 - idx);

		mesh->vertices[vidx].position.x = std::stof(substr);

		idx = line.find_first_of(' ', idx1);
		idx1 = line.find_first_of(',', idx);
		substr = line.substr(idx, idx1 - idx);

		mesh->vertices[vidx].position.y = std::stof(substr);

		idx = line.find_first_of(' ', idx1);
		idx1 = line.find_first_of('}', idx);
		substr = line.substr(idx, idx1 - idx);

		mesh->vertices[vidx].position.z = std::stof(substr);

		vidx++;
	}
}

void MDLLoader::parse_indices(std::ifstream& stream, Mesh* mesh)
{
	std::string line, substr;

	unsigned int iidx = 0;
	while (iidx < mesh->indices.size())
	{
		int count = 0;

		std::getline(stream, line);

		auto idx = line.find_first_not_of(' ', 0);
		auto idx1 = line.find_first_of(',', idx);

		do
		{
			substr = line.substr(idx, idx1 - idx);
			mesh->indices[iidx] = std::stoi(substr);
			iidx++;

			idx = line.find_first_of(' ', idx1);
			idx1 = line.find_first_of(',', idx);

			if (idx1 == std::string::npos)
				break;
		} 
		while (true);
	}
}

void MDLLoader::parse_normals(std::ifstream& stream, Mesh* mesh)
{
	std::string line, substr;

	unsigned int vidx = 0;
	while (vidx < mesh->vertices.size())
	{
		std::getline(stream, line);

		auto idx = line.find_first_of(' ', 0);
		auto idx1 = line.find_first_of(',', idx);
		substr = line.substr(idx, idx1 - idx);

		mesh->vertices[vidx].normal.x = std::stof(substr);

		idx = line.find_first_of(' ', idx1);
		idx1 = line.find_first_of(',', idx);
		substr = line.substr(idx, idx1 - idx);

		mesh->vertices[vidx].normal.y = std::stof(substr);

		idx = line.find_first_of(' ', idx1);
		idx1 = line.find_first_of('}', idx);
		substr = line.substr(idx, idx1 - idx);

		mesh->vertices[vidx].normal.z = std::stof(substr);

		vidx++;
	}
}

void MDLLoader::parse_uvs(std::ifstream& stream, Mesh* mesh)
{
	std::string line, substr;

	unsigned int vidx = 0;
	while (vidx < mesh->vertices.size())
	{
		std::getline(stream, line);

		auto idx = line.find_first_of(' ', 0);
		auto idx1 = line.find_first_of(',', idx);
		substr = line.substr(idx, idx1 - idx);

		mesh->vertices[vidx].uv.x = std::stof(substr);

		idx = line.find_first_of(' ', idx1);
		idx1 = line.find_first_of('}', idx);
		substr = line.substr(idx, idx1 - idx);

		mesh->vertices[vidx].uv.y = std::stof(substr);

		vidx++;
	}
}

void MDLLoader::parse_texture(std::ifstream& stream, MDLFile* mdl)
{
	std::string line, value;

	while (std::getline(stream, line))
	{
		if (line.find_first_not_of(' ') == std::string::npos)
			break;

		auto idx1 = line.find_first_not_of('\t', 0);
		auto idx2 = line.find_first_of(' ');

		value = line.substr(idx1, idx2 - idx1);

		if (value == "Texture")
		{
			std::getline(stream, line);

			idx1 = line.find("Image");
			idx1 = line.find_first_of(' ', idx1);
			value = line.substr(idx1 + 1, line.size() - idx1);

			unsigned int texture_id = Services::get().get_asset_manager()->load_asset<Texture>(value.c_str());
			Texture* texture = &static_cast<Texture&>(Services::get().get_asset_manager()->get_asset(texture_id));

			mdl->_textures.push_back(texture);
		}
	}
}

void MDLLoader::parse_transform(std::ifstream& stream, Mesh* mesh)
{
	glm::mat4 transform = glm::mat4(1.0f);
	std::string line, value;
	float value1, value2, value3;
	
	// Parse position
	std::getline(stream, line);

	auto idx1 = line.find_first_of('{', 0);
	idx1++;
	auto idx2 = line.find_first_of(',', idx1);
	value1 = std::stof(line.substr(idx1, idx2 - idx1));

	idx1 = line.find_first_of(' ', idx2);
	idx2 = line.find_first_of(',', idx1);
	value2 = std::stof(line.substr(idx1, idx2 - idx1));

	idx1 = line.find_first_of(' ', idx2);
	idx2 = line.find_first_of('}', idx1);
	value3 = std::stof(line.substr(idx1, idx2 - idx1));

	transform = glm::translate(transform, glm::vec3(value1, value2, value3));

	// Parse rotation
	std::getline(stream, line);

	idx1 = line.find_first_of('{', 0);
	idx1++;
	idx2 = line.find_first_of(',', idx1);
	value1 = std::stof(line.substr(idx1, idx2 - idx1));

	idx1 = line.find_first_of(' ', idx2);
	idx2 = line.find_first_of(',', idx1);
	value2 = std::stof(line.substr(idx1, idx2 - idx1));

	idx1 = line.find_first_of(' ', idx2);
	idx2 = line.find_first_of('}', idx1);
	value3 = std::stof(line.substr(idx1, idx2 - idx1));

	transform = glm::rotate(transform, glm::radians(value1), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(value2), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(value3), glm::vec3(0.0f, 0.0f, 1.0f));

	for (int i = 0; i < mesh->vertices.size(); i++)
	{
		glm::vec4 new_pos = transform * glm::vec4(mesh->vertices[i].position, 1.0f);
		mesh->vertices[i].position = new_pos;
	}
}