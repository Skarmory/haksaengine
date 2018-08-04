#pragma once

#include <string>

#include "asset_manager.h"

#include "gfx/texture.h"

class Tileset : public Asset
{
public:

	Tileset(void);
	Tileset(const std::string& name, unsigned int width, unsigned int height, unsigned int tile_size);

	unsigned int width(void) const;
	unsigned int height(void) const;
	unsigned int tile_size(void) const;

	void use(void);

	const Texture* get_texture(void) const;

	void load(const std::string& name, unsigned int width, unsigned int height, unsigned int tile_size);

private:

	unsigned int _width;
	unsigned int _height;
	unsigned int _tile_size;

	const Texture* _tileset_texture;
};