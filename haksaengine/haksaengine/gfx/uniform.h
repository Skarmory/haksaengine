#pragma once

#include <glm/glm.hpp>

#include <string>

// Abstract class for polymorphism of derived templated class
class BaseBuffer
{
public:

	virtual ~BaseBuffer(void) = default;

	virtual const void* get_data(void) const = 0;
	virtual std::size_t get_size(void) const = 0;
	virtual unsigned int get_location(void) const = 0;
};

template<class BufferDataType>
class Buffer : public BaseBuffer
{
public:

	// Create without data, but at a given binding location
	explicit Buffer(unsigned int bind_point) : _location(bind_point)
	{
	}

	// Create with given data at given binding location
	Buffer(unsigned int bind_point, const BufferDataType* data, unsigned int count = 1) : _location(bind_point)
	{
		set_data(data, count);
	}

	~Buffer(void)
	{
		if (_data)
		{
			if (_count == 1)
				delete _data;
			else
				delete[] _data;
		}
	}

	// Set data and amount if it's an array
	void set_data(const BufferDataType* data, unsigned int count = 1)
	{
		if (_data)
			delete _data;

		_count = count;
		_data = static_cast<BufferDataType*>(malloc(sizeof(BufferDataType) * _count));

		std::memcpy(_data, data, sizeof(BufferDataType) * _count);
	}

	// Get data pointer
	const void* get_data(void) const override
	{
		return static_cast<const void*>(_data);
	}

	// Get size of the data in bytes (if array, it's the total array size)
	std::size_t get_size(void) const override
	{
		return sizeof(BufferDataType) * _count;
	}

	// Get binding location
	unsigned int get_location(void) const override
	{
		return _location;
	}

private:
	unsigned int _location;
	unsigned int _count;
	BufferDataType* _data;
};