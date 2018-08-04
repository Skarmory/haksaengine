#pragma once

#include <GL/glew.h>

#include "gfx/storage_buffer.h"

class Renderer;

// Class that wraps the logic for creating and updating a uniform buffer on GPU
class ShaderStorageBufferObject : public StorageBufferObject
{
	friend class Renderer;

public:

	explicit ShaderStorageBufferObject(void) = default;

	// No copying
	ShaderStorageBufferObject(const ShaderStorageBufferObject& other) = delete;
	ShaderStorageBufferObject& operator=(const ShaderStorageBufferObject& other) = delete;

	// Bind this buffer for use
	void bind(void) const override
	{
		if (!_initialised)
			throw std::runtime_error("Attempt to bind uninitialised UniformBufferObject");

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _location, _buffer);
	}
};