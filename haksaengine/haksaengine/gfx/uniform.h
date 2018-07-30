#pragma once

#include <glm/glm.hpp>

#include <string>

// Abstract class for polymorphism of derived templated class
class BaseUniform
{
public:

	virtual ~BaseUniform(void) = default;

	virtual const void* get_data(void) const = 0;
	virtual std::size_t get_size(void) const = 0;
	virtual unsigned int get_location(void) const = 0;
};

template<class UniformType>
class Uniform : public BaseUniform
{
public:

	// Create uniform without data, but at a given binding location
	explicit Uniform(unsigned int bind_point) : _location(bind_point)
	{
	}

	// Create uniform with given data at given binding location
	Uniform(unsigned int bind_point, const UniformType* data, unsigned int count = 1) : _location(bind_point)
	{
		set_data(data, count);
	}

	~Uniform(void)
	{
		if (_uniform)
			delete _uniform;
	}

	// Set uniform data
	void set_data(const UniformType* uniform, unsigned int count = 1)
	{
		if (_uniform)
			delete _uniform;

		_count = count;
		_uniform = static_cast<UniformType*>(malloc(sizeof(UniformType) * _count));

		std::memcpy(_uniform, uniform, sizeof(UniformType) * _count);
	}

	// Get uniform data pointer
	const void* get_data(void) const override
	{
		return static_cast<const void*>(_uniform);
	}

	// Get size of the data in bytes
	std::size_t get_size(void) const override
	{
		return sizeof(UniformType) * _count;
	}

	// Get binding location
	unsigned int get_location(void) const override
	{
		return _location;
	}

private:
	unsigned int _location;
	unsigned int _count;
	UniformType* _uniform;
};