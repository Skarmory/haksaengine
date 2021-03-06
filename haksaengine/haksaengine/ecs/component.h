#pragma once

#include <fstream>

#include "globals.h"

#include "variant.h"

// Base component for polymorphic usage
class BaseComponent
{
public:
	virtual ~BaseComponent(void) = default;
	virtual BaseComponent* clone(void) const = 0;
	virtual void save(std::ofstream& os) const = 0;
	virtual void load(NamedVariantPack* data) = 0;

	bool is_transient(void) const
	{
		return _transient;
	}

protected:

	bool _transient;
};

/* The component class contains the data that is attached to an entity. */
/* This should be derived from for each logical grouping of data. */
template<class Derived>
class Component : public BaseComponent
{
public:
	Component(void)
	{
		_transient = false;
	}

	virtual BaseComponent* clone(void) const override
	{
		return new Derived(static_cast<const Derived&>(*this));
	}

	virtual void save(std::ofstream& os) const override
	{
		// Do nothing
	}

	virtual void load(NamedVariantPack* data) override
	{
		// Do nothing
	}
};
