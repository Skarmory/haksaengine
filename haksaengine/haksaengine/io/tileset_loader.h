#pragma once

#include "io/loader.h"

#include "gfx/tileset.h"

class TilesetLoader : public Loader
{
public:

	TilesetLoader(const std::string& directory);

	Tileset* load(const std::string& name) override;
};