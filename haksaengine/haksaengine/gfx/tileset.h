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

	HAKSAENGINE_API unsigned int width(void) const;
	HAKSAENGINE_API unsigned int height(void) const;
	HAKSAENGINE_API unsigned int tile_count(void) const;
	HAKSAENGINE_API unsigned int tile_size(void) const;

	void use(void);

	HAKSAENGINE_API const ArrayTexture* get_texture(void) const;

	HAKSAENGINE_API std::string texture_path(unsigned int id) const;

private:

	unsigned int _width;
	unsigned int _height;
	unsigned int _tile_count;
	unsigned int _tile_size;

	std::vector<std::string> _texture_paths;

	ArrayTexture* _tileset_texture;
};