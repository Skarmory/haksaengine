#pragma once

#include "GL/glew.h"

#include "asset_manager.h"

typedef GLuint64 BindlessTextureHandle;

enum class FilterMode
{
	Nearest = 0,
	Linear  = 1
};

enum class TextureFormat
{
	UnsignedByte = 0,
	Float        = 1
};

// Wraps a texture image and OpenGL objects
class Texture : public Asset
{
	friend class TextureLoader;

public:
	Texture(void* data, unsigned int data_size, unsigned int width, unsigned int height, FilterMode filter_mode, TextureFormat format, bool gen_mip_maps);
	~Texture(void);

	// No copying
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	// Get texture handle (for bindless textures)
	BindlessTextureHandle get_handle(void) const;

	// Texture width
	unsigned int get_width(void) const;

	// Texture height
	unsigned int get_height(void) const;

	// Initialise the texture on the GPU
	void initialise(void);

	// Uninitialise the texture on the GPU
	void uninitialise(void);

private:
	bool _initialised;
	bool _gen_mip_maps;

	unsigned int _width;
	unsigned int _height;

	FilterMode _filter_mode;
	TextureFormat _format;

	void* _data;

	GLuint _texture;

	// Bind this texture to the specified binding index
	void bind(int binding) const;	
};