#include "io/tileset_loader.h"

#include <string>

#include "services.h"
#include "lodepng.h"

TilesetLoader::TilesetLoader(const std::string& directory) : Loader(directory, ".tileset")
{
}

Tileset* TilesetLoader::load(const std::string& name)
{
	std::string path = get_path(name);

	std::ifstream stream(path);

	std::string line, value;

	Tileset* tileset = new Tileset;

	while (std::getline(stream, line))
	{
		if (line.empty())
			continue;

		auto idx1 = line.find(' ');

		value = line.substr(0, idx1);

		if (value == "Info")
		{
			_parse_info(stream, tileset);
		}
		else if (value == "Tiles")
		{
			_parse_tiles(stream, tileset);
		}
		else
		{
			throw std::runtime_error("Unexpected section in tileset");
		}
	}

	tileset->_tileset_texture->initialise();

	return tileset;
}

void TilesetLoader::_parse_info(std::ifstream& stream, Tileset* tileset)
{
	std::string line, value;

	while (std::getline(stream, line))
	{
		if (line.find('}') != std::string::npos)
			return;

		auto idx1 = line.find_first_not_of('\t');
		auto idx2 = line.find_first_of(' ', idx1);

		value = line.substr(idx1, idx2 - idx1);

		if (value == "Width")
		{
			idx1 = line.find_first_not_of(' ', idx2);
			
			value = line.substr(idx1, line.size() - idx1);

			tileset->_width = std::stoul(value);
		}
		else if (value == "Height")
		{
			idx1 = line.find_first_not_of(' ', idx2);

			value = line.substr(idx1, line.size() - idx1);

			tileset->_height = std::stoul(value);
		}
		else if (value == "Count")
		{
			idx1 = line.find_first_not_of(' ', idx2);

			value = line.substr(idx1, line.size() - idx1);

			tileset->_tile_count = std::stoul(value);
		}
		else
		{
			throw std::runtime_error("Unexpected section in tileset info block");
		}
	}
}

void TilesetLoader::_parse_tiles(std::ifstream& stream, Tileset* tileset)
{
	std::string line, value;
	unsigned int tile_idx = 0;
	unsigned int width, height;
	std::vector<unsigned char> data;
	data.reserve(tileset->_width * tileset->_height * 4);

	std::string dir = std::string(Services::get().get_asset_manager()->get_asset_directory_path()) + "textures/";

	tileset->_tileset_texture = new ArrayTexture(
		nullptr, 0, tileset->_width, tileset->_height, tileset->_tile_count,
		FilterMode::Linear, TextureFormat::UnsignedByte, true
	);

	while (std::getline(stream, line))
	{
		if (line.find('}') != std::string::npos)
			return;

		auto idx1 = line.find_first_not_of('\t');
		auto idx2 = line.find_first_of(' ', idx1);

		value = line.substr(idx1, idx2 - idx1);

		if (value == "Path")
		{
			idx1 = line.find_first_not_of(' ', idx2);

			value = line.substr(idx1, line.size() - idx1);

			data.clear();

			std::string path = dir + value;

			lodepng::decode(data, width, height, path);

			tileset->_tileset_texture->update(data.data(), data.size(), tile_idx);

			tile_idx++;
		}
		else
		{
			throw std::runtime_error("Unexpected section in tileset info block");
		}
	}
}