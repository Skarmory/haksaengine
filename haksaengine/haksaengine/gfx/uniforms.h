#pragma once

#include <glm/glm.hpp>

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
	Uniform(unsigned int bind_point, const UniformType& data) : _location(bind_point), _uniform(data)
	{
	}

	// Set uniform data
	void set_data(const UniformType& uniform)
	{
		_uniform = uniform;
	}

	// Get uniform data pointer
	const void* get_data(void) const override
	{
		return static_cast<const void*>(&_uniform);
	}

	// Get size of the data in bytes
	std::size_t get_size(void) const override
	{
		return sizeof(UniformType);
	}

	// Get binding location
	unsigned int get_location(void) const override
	{
		return _location;
	}

private:
	unsigned int _location;
	UniformType _uniform;
};