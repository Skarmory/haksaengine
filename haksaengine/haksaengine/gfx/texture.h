#pragma once

#include "GL/glew.h"

#include "asset_manager.h"

typedef GLuint64 BindlessTextureHandle;

// Wraps a texture image and OpenGL objects
class Texture : public Asset
{
	friend class TextureLoader;

public:
	Texture(void);
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

private:
	bool _initialised;

	unsigned int _width;
	unsigned int _height;

	std::vector<unsigned char> _image;

	GLuint _texture;

	// Initialise the texture on the GPU
	void initialise(void);

	// Uninitialise the texture on the GPU
	void uninitialise(void);

	// Bind this texture to the specified binding index
	void bind(int binding) const;	
};