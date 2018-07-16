#pragma once

#include <GL/glew.h>
#include "gfx/uniform.h"

class Renderer;
class BaseUniform;

// Class that wraps the logic for creating and updating a uniform buffer on GPU
class UniformBufferObject
{
public:

	// Create a UBO
	explicit UniformBufferObject(void) : _initialised(false)
	{
	}

	// Deconstruct UBO. Destroys it in GPU memory too.
	~UniformBufferObject(void)
	{
		if (_initialised)
			uninitialise();
	}

	// No copying
	UniformBufferObject(const UniformBufferObject& other) = delete;
	UniformBufferObject& operator=(const UniformBufferObject& other) = delete;

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

	// Update the data in this UBO on GPU
	void update(const BaseUniform* data)
	{
		if (!_initialised)
			throw std::runtime_error("Attempt to update uninitialised UniformBufferObject");

		glNamedBufferData(_buffer, data->get_size(), data->get_data(), GL_DYNAMIC_DRAW);
	}

	// Bind this buffer for use
	void bind(void) const
	{
		if (!_initialised)
			throw std::runtime_error("Attempt to bind uninitialised UniformBufferObject");

		glBindBufferBase(GL_UNIFORM_BUFFER, _location, _buffer);
	}

private:
	bool _initialised;
	GLuint _buffer;
	GLuint _location;

	friend class Renderer;
};
