#pragma once

#include <string>

#include "asset_manager.h"

#include "gfx/texture.h"

class Tileset
{
public:

	Tileset(const std::string& name, unsigned int width, unsigned int height, unsigned int tile_size);

	unsigned int width(void) const;
	unsigned int height(void) const;
	unsigned int tile_size(void) const;

	void use(void);

private:

	unsigned int _width;
	unsigned int _height;
	unsigned int _tile_size;

	const Texture* _tileset_texture;
};