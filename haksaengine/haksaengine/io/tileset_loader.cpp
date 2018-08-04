#include "io/tileset_loader.h"

TilesetLoader::TilesetLoader(const std::string& directory) : Loader(directory, ".tileset")
{
}

Tileset* TilesetLoader::load(const std::string& name)
{
	Tileset* tileset = new Tileset;

	return tileset;
}