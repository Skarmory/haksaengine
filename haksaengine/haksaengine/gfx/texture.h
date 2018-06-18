#pragma once

#include "GL/glew.h"

#include "asset_manager.h"

class Texture : public Asset
{
public:
	Texture(void);
	~Texture(void);

	// No copying
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	// Initialise the texture on the GPU
	void initialise(void);

	// Uninitialise the texture on the GPU
	void uninitialise(void);

	// Bind this texture to the specified binding index
	void bind(int binding) const;

	// Texture width
	unsigned int get_width(void) const;

	// Texture height
	unsigned int get_height(void) const;

private:
	bool initialised;

	unsigned int width, height;
	std::vector<unsigned char> image;

	int binding;
	GLuint texture;

	friend class TextureLoader;
};