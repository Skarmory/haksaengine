#pragma once

#include <glm/glm.hpp>

#include "gfx/texture.h"

// Maximum amount of bones a model can have, corresponding to shader uniform
#define BONES_MAX 40

// Each predefined uniform block binding location
#define CAMERA_UNIFORM_BIND_POINT   0
#define PER_DRAW_UNIFORM_BIND_POINT 1

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

struct CameraData
{
	glm::mat4 view;
	glm::mat4 projection;
};

struct PerDrawData
{
	glm::mat4    model;
	float        alpha;
	unsigned int player_colour;

	BindlessTextureHandle diffuse;
};

struct PerDrawDataSkinned
{
	glm::mat4    model;
	glm::mat4    bones[BONES_MAX];
	float        alpha;
	unsigned int player_colour;

	BindlessTextureHandle diffuse;
};