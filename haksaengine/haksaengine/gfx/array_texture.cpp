#include "gfx/array_texture.h"

ArrayTexture::ArrayTexture(void* data, unsigned int data_size, unsigned int width, unsigned int height, unsigned int layers, FilterMode filter_mode, TextureFormat format, bool gen_mip_maps)
	: _initialised(false), _gen_mip_maps(gen_mip_maps), _width(width), _height(height), _layers(layers), _filter_mode(filter_mode), _format(format), _texture(0)
{
	if (format == TextureFormat::UnsignedByte)
		_type_size = sizeof(unsigned char);
	else
		_type_size = sizeof(float);

	_data = malloc(width * height * layers * _type_size * 4);

	if(data)
		memcpy(_data, data, data_size);
}

ArrayTexture::~ArrayTexture(void)
{
	if (_initialised)
		uninitialise();

	if (_data)
		delete _data;
}

void ArrayTexture::initialise(void)
{
	if (!_initialised)
	{
		// Just do some very basic setup for now
		glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &_texture);

		glTextureStorage3D(_texture, 1, GL_RGBA, _width, _height, _layers);

		glBindTexture(GL_TEXTURE_2D_ARRAY, _texture);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

		int filter_mode = _filter_mode == FilterMode::Nearest ? GL_NEAREST : GL_LINEAR_MIPMAP_LINEAR;

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, filter_mode);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, filter_mode);

		int type = _format == TextureFormat::UnsignedByte ? GL_UNSIGNED_BYTE : GL_FLOAT;
		int internal = _format == TextureFormat::UnsignedByte ? GL_RGBA : GL_RGBA32F_ARB;
		int format = GL_RGBA;

		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internal, _width, _height, _layers, 0, format, type, _data);

		if (_gen_mip_maps)
			glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

		_initialised = true;
	}
}

void ArrayTexture::uninitialise(void)
{
	if (_initialised)
		glDeleteTextures(1, &_texture);

	_initialised = false;
}

void ArrayTexture::bind(int binding) const
{
	if (!_initialised)
		return;

	glBindTextureUnit(binding, _texture);
}

BindlessTextureHandle ArrayTexture::get_handle(void) const
{
	if (!_initialised)
		return 0;

	return glGetTextureHandleARB(_texture);
}

unsigned int ArrayTexture::get_width(void) const
{
	return _width;
}

unsigned int ArrayTexture::get_height(void) const
{
	return _height;
}

unsigned int ArrayTexture::get_layers(void) const
{
	return _layers;
}

void ArrayTexture::update(void* data, std::size_t size, unsigned int layer)
{
	unsigned int offset = _width * _height * _type_size * 4 * layer;

	memcpy(static_cast<char*>(_data) + offset, data, size);
}