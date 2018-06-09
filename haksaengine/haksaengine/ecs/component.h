#pragma once

#include "globals.h"

class BaseComponent
{
public:
	virtual BaseComponent* clone(void) const = 0;
};

/* The component class contains the data that is attached to an entity. */
/* This should be derived from for each logical grouping of data. */
template<class Derived>
class Component : public BaseComponent
{
public:
	Component(void) = default;
	virtual ~Component(void) = default;

	virtual BaseComponent* clone(void) const override
	{
		return new Derived(static_cast<const Derived&>(*this));
	}
};
