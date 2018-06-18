#include "gfx/texture.h"

Texture::Texture(void) : initialised(false), texture(0)
{
}

Texture::~Texture(void)
{
	if (initialised)
		uninitialise();
}

void Texture::initialise(void)
{
	if (!initialised)
	{
		// Just do some very basic setup for now
		glCreateTextures(GL_TEXTURE_2D, 1, &texture);

		glTextureStorage2D(texture, 1, GL_RGBA, width, height);

		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		initialised = true;
	}
}

void Texture::uninitialise(void)
{
	if (initialised)
		glDeleteTextures(1, &texture);

	initialised = false;
}

void Texture::bind(int binding) const
{
	glBindTextureUnit(binding, texture);
}

unsigned int Texture::get_width(void) const
{
	return width;
}

unsigned int Texture::get_height(void) const
{
	return height;
}