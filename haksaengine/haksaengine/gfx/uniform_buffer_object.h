#pragma once

#include <GL/glew.h>
#include "gfx/storage_buffer.h"
#include "gfx/uniform.h"

class Renderer;
class BaseBuffer;

// Class that wraps the logic for creating and updating a uniform buffer on GPU
class UniformBufferObject : public StorageBufferObject
{
	friend class Renderer;

public:

	explicit UniformBufferObject(void) = default;

	// No copying
	UniformBufferObject(const UniformBufferObject& other) = delete;
	UniformBufferObject& operator=(const UniformBufferObject& other) = delete;

	// Bind this buffer for use
	void bind(void) const override
	{
		if (!_initialised)
			throw std::runtime_error("Attempt to bind uninitialised UniformBufferObject");

		glBindBufferBase(GL_UNIFORM_BUFFER, _location, _buffer);
	}
};