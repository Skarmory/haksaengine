#pragma once

#include <string>

#include "asset_manager.h"

#include "gfx/array_texture.h"

class Tileset : public Asset
{
	friend class TilesetLoader;

public:

	Tileset(void);
	Tileset(const std::string& name, unsigned int width, unsigned int height, unsigned int tile_count, unsigned int tile_size);
	~Tileset(void);

	unsigned int width(void) const;
	unsigned int height(void) const;
	unsigned int tile_count(void) const;
	unsigned int tile_size(void) const;

	void use(void);

	const ArrayTexture* get_texture(void) const;

	void load(const std::string& name, unsigned int width, unsigned int height, unsigned int tile_size);

private:

	unsigned int _width;
	unsigned int _height;
	unsigned int _tile_count;
	unsigned int _tile_size;

	ArrayTexture* _tileset_texture;
};