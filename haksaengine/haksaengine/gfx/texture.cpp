#include "gfx/texture.h"

Texture::Texture(void* data, unsigned int data_size, unsigned int width, unsigned int height, FilterMode filter_mode, TextureFormat format, bool gen_mip_maps) 
	: _initialised(false), _gen_mip_maps(gen_mip_maps), _width(width), _height(height), _filter_mode(filter_mode), _format(format), _texture(0)
{
	unsigned int size;

	if (format == TextureFormat::UnsignedByte)
		size = sizeof(unsigned char);
	else
		size = sizeof(float);

	_data = malloc(width * height * size * 4);
	
	memcpy(_data, data, data_size);
}

Texture::~Texture(void)
{
	if (_initialised)
		uninitialise();

	if(_data)
		delete _data;
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

		int filter_mode = _filter_mode == FilterMode::Nearest ? GL_NEAREST : GL_LINEAR;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_mode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mode);

		int type = _format == TextureFormat::UnsignedByte ? GL_UNSIGNED_BYTE : GL_FLOAT;
		int internal = _format == TextureFormat::UnsignedByte ? GL_RGBA : GL_RGBA32F_ARB;
		int format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, internal, _width, _height, 0, format, type, _data);

		if(_gen_mip_maps)
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