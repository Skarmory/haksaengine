#include "gfx/mesh_loader.h"

#include <string>
#include <fstream>

Mesh* MeshLoader::load(const std::string& id)
{
	Mesh* mesh = new Mesh;
	std::ifstream fs;
	std::string line;

	fs.open(id);

	while (std::getline(fs, line))
	{
		if (line.find_first_not_of(' ') == std::string::npos)
			continue;

		auto idx = line.find_first_of(' ');
		auto value = line.substr(0, idx);

		if (value == "Vertices")
		{
			idx = line.find_first_not_of(' ', idx);
			auto idx1 = line.find_first_of(' ', idx);
			value = line.substr(idx, idx1-idx);
			mesh->vertices.resize(std::stoi(value));
			parse_vertices(fs, mesh);
		}
		else if (value == "Indices")
		{
			idx = line.find_first_not_of(' ', idx);
			auto idx1 = line.find_first_of(' ', idx);
			value = line.substr(idx, idx1 - idx);
			mesh->indices.resize(std::stoi(value));
			parse_indices(fs, mesh);
		}
		else if (value == "Normals")
		{			
			parse_normals(fs, mesh);
		}
	}


	fs.close();

	return mesh;
}

void MeshLoader::parse_vertices(std::ifstream& stream, Mesh* mesh)
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

void MeshLoader::parse_indices(std::ifstream& stream, Mesh* mesh)
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

void MeshLoader::parse_normals(std::ifstream& stream, Mesh* mesh)
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