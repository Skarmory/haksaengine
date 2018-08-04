#pragma once

#pragma once

#include <GL/glew.h>
#include "gfx/uniform.h"

class Renderer;

// Class that wraps the logic for creating and updating a uniform buffer on GPU
class StorageBufferObject
{
	friend class Renderer;

public:

	// Create a UBO
	explicit StorageBufferObject(void) : _initialised(false), _size(0)
	{
	}

	// Deconstruct UBO. Destroys it in GPU memory too.
	virtual ~StorageBufferObject(void)
	{
		if (_initialised)
			uninitialise();
	}

	// No copying
	StorageBufferObject(const StorageBufferObject& other) = delete;
	StorageBufferObject& operator=(const StorageBufferObject& other) = delete;

	// Initialise this UBO on GPU
	void initialise(void)
	{
		if (_initialised)
			return;

		glCreateBuffers(1, &_buffer);

		_initialised = true;
	}

	// Destroy the GPU memory
	void uninitialise(void)
	{
		if (!_initialised)
			return;

		glDeleteBuffers(1, &_buffer);
	}

	// Update the data in this buffer on GPU
	void update(const void* data, std::size_t size, std::size_t offset = 0)
	{
		if (!_initialised)
			throw std::runtime_error("Attempt to update uninitialised UniformBufferObject");

		//unsigned int data_size = data->get_size();

		// glNamedBufferData completely reallocates memory, so only do that if we actually need more
		// glNamedBufferSubData just writes new values into already allocated memory
		if (size > _size)
		{
			_size = size;
			glNamedBufferData(_buffer, size, data, GL_DYNAMIC_DRAW);
		}
		else
		{
			glNamedBufferSubData(_buffer, offset, size, data);
		}
	}

	// Bind this buffer for use
	virtual void bind(void) const = 0;

protected:
	bool _initialised;
	GLuint _buffer;
	GLuint _location;
	unsigned int _size;
};