#pragma once

#include <GL/glew.h>

// Class that wraps the logic for creating and updating a uniform buffer on GPU
template<class BufferDataType>
class UniformBufferObject
{
public:

	// Create a UBO for a particular bind point
	// The binding point is based on how it is laid out in the shader (e.g. layout(binding = 0)..)
	explicit UniformBufferObject(unsigned int bind_point) : _initialised(false), _bind_point(bind_point)
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

	// Update the data in this UBO
	void update(const BufferDataType& data)
	{
		glNamedBufferData(_buffer, sizeof(BufferDataType), &data, GL_DYNAMIC_DRAW);
	}

	// Bind this buffer for use
	void bind(void) const
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, _bind_point, _buffer);
	}

private:
	bool _initialised;
	GLuint _buffer;
	GLuint _bind_point;
};
