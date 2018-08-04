#include "gfx/tileset.h"

#include "services.h"
#include "gfx/render_command.h"

Tileset::Tileset(void) : _width(0), _height(0), _tile_size(0), _tileset_texture(nullptr)
{
}

Tileset::Tileset(const std::string& name, unsigned int width, unsigned int height, unsigned int tile_size)
	: _width(width), _height(height), _tile_size(tile_size)
{
	_tileset_texture = &Services::get().get_asset_manager()->load_and_get_asset<Texture>(name.c_str());
}

unsigned int Tileset::width(void) const
{
	return _width;
}

unsigned int Tileset::height(void) const
{
	return _height;
}

unsigned int Tileset::tile_size(void) const
{
	return _tile_size;
}

void Tileset::use(void)
{
	MakeTextureHandlesResidentCommand* cmd = new MakeTextureHandlesResidentCommand;
	cmd->add_texture_handle(_tileset_texture->get_handle());

	std::vector<const RenderCommand*> cmds;
	cmds.push_back(cmd);

	Services::get().get_renderer()->submit_render_commands(cmds);
}

const Texture* Tileset::get_texture(void) const
{
	return _tileset_texture;
}

void Tileset::load(const std::string& name, unsigned int width, unsigned int height, unsigned int tile_size)
{
	_width = width;
	_height = height;
	_tile_size = tile_size;
	_tileset_texture = &Services::get().get_asset_manager()->load_and_get_asset<Texture>(name.c_str());
}