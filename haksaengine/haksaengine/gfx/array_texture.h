#pragma once

#include "GL/glew.h"

#include "asset_manager.h"

#include "gfx/texture.h"

// Wraps a texture image and OpenGL objects
class ArrayTexture : public Asset
{

public:
	ArrayTexture(void* data, unsigned int data_size, unsigned int width, unsigned int height, unsigned int layers, FilterMode filter_mode, TextureFormat format, bool gen_mip_maps);
	~ArrayTexture(void);

	// No copying
	ArrayTexture(const ArrayTexture&) = delete;
	ArrayTexture& operator=(const ArrayTexture&) = delete;

	// Get texture handle (for bindless textures)
	BindlessTextureHandle get_handle(void) const;

	// Texture width
	unsigned int get_width(void) const;

	// Texture height
	unsigned int get_height(void) const;

	// Get layers
	unsigned int get_layers(void) const;

	// Initialise the texture on the GPU
	void initialise(void);

	// Uninitialise the texture on the GPU
	void uninitialise(void);

	// Updates one layer of data
	void update(void* data, std::size_t size, unsigned int layer);

private:
	bool _initialised;
	bool _gen_mip_maps;

	unsigned int _type_size;
	unsigned int _width;
	unsigned int _height;
	unsigned int _layers;

	FilterMode _filter_mode;
	TextureFormat _format;

	void* _data;

	GLuint _texture;

	// Bind this texture to the specified binding index
	void bind(int binding) const;
};