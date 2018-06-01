#pragma once

#include <unordered_map>
#include <typeindex>
#include <algorithm>

#include "globals.h"
#include "component.h"

/* The entity class serves as the logical grouping for several components. */
/* In essence, it represents any object or behaviour in a game, from a character to a spawn system. What is represents is determined by what components are attached to it. */
class HAKSAENGINE_API Entity
{
public:
	Entity(uint32_t id);
	~Entity(void);

	// Adds a component to the entity
	template<class C>
	void add_component(Component* component)
	{
		if (!has_component<C>())
			_components[typeid(C)] = component;
	}

	// Removes a component from the entity
	template<class C>
	void  remove_component(void)
	{
		_components.erase(typeid(C));
	}

	// Check if the entity has a given component
	template<class C>
	bool has_component(void)
	{
		return _components.find(typeid(C)) != _components.end();
	}

private:
	uint32_t _id;
	std::unordered_map<std::type_index, Component*> _components;
};