#include "gfx/texture.h"

Texture::Texture(void) : _initialised(false), _texture(0), _width(0), _height(0)
{
}

Texture::~Texture(void)
{
	if (_initialised)
		uninitialise();
}

void Texture::initialise(void)
{
	if (!_initialised)
	{
		// Just do some very basic setup for now
		glCreateTextures(GL_TEXTURE_2D, 1, &_texture);

		glTextureStorage2D(_texture, 1, GL_RGBA, _width, _height);

		glBindTexture(GL_TEXTURE_2D, _texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _image.data());

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		_initialised = true;
	}
}

void Texture::uninitialise(void)
{
	if (_initialised)
		glDeleteTextures(1, &_texture);

	_initialised = false;
}

void Texture::bind(int binding) const
{
	if (!_initialised)
		return;

	glBindTextureUnit(binding, _texture);
}

BindlessTextureHandle Texture::get_handle(void) const
{
	if (!_initialised)
		return 0;

	return glGetTextureHandleARB(_texture);
}

unsigned int Texture::get_width(void) const
{
	return _width;
}

unsigned int Texture::get_height(void) const
{
	return _height;
}