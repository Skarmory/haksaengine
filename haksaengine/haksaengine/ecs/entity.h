#pragma once

#include <unordered_map>
#include <typeindex>
#include <algorithm>

#include "globals.h"
#include "component.h"

/* The entity class serves as the logical grouping for several components. */
/* In essence, it represents any object or behaviour in a game, from a character to a spawn system. What is represents is determined by what components are attached to it. */
class Entity
{
public:
	HAKSAENGINE_API Entity(void);
	HAKSAENGINE_API Entity(unsigned int id);
	HAKSAENGINE_API ~Entity(void);

	// Adds a component to the entity
	void add_component(BaseComponent* component)
	{
		_components[typeid(*component)] = component;
	}

	// Removes a component from the entity
	template<class C>
	void remove_component(void)
	{
		_components.erase(typeid(C));
	}

	// Get a component from the entity if it exists, else return nullptr
	template<class C>
	C* get_component(void) const
	{
		if(_components.find(typeid(C)) != _components.end())
			return static_cast<C*>(_components.at(typeid(C)));
		
		return nullptr;
	}

	// Check if the entity has a given component
	template<class C>
	bool has_component(void)
	{
		return _components.find(typeid(C)) != _components.end();
	}

	// Get this entity's id
	unsigned int get_id(void) const
	{
		return _id;
	}

private:
	unsigned int _id;
	std::unordered_map<std::type_index, BaseComponent*> _components;
};