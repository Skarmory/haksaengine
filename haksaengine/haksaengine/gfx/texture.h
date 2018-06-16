#pragma once

#include "GL/glew.h"

class Texture
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

	// Unbind this texture
	void unbind(void) const;

private:
	bool initialised;

	GLuint texture;
};