#pragma once

#include <vector>
#include <fstream>

#include "io/loader.h"

#include "gfx/tileset.h"

class TilesetLoader : public Loader
{
public:

	TilesetLoader(const std::string& directory);

	Tileset* load(const std::string& name) override;

private:

	void _parse_info(std::ifstream& stream, Tileset* tileset);

	void _parse_tiles(std::ifstream& stream, Tileset* tileset);
};