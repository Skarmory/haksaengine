#include "ecs/entity_manager.h"

#include "services.h"
#include <iostream>

EntityManager::EntityManager(void) : next_id(0)
{
}

EntityManager::~EntityManager(void)
{
}

unsigned int EntityManager::create_entity(std::vector<BaseComponent*>* const components)
{
	_entities[next_id] = Entity(next_id);

	if (components)
	{
		for (auto component : *components)
		{
			_entities[next_id].add_component(component);
		}
	}

	Variant v;
	v.type = Variant::Type::UNSIGNEDINT;
	v.as_uint = next_id;

	Event ev;
	ev.event_type = "EntityCreatedEvent";
	ev.arguments.push_back(v);

	Services::get().get_event_manager()->dispatch(ev);

	return next_id++;
}

void EntityManager::destroy_entity(unsigned int id)
{
	if (_entities.find(id) != _entities.end())
	{
		_entities.erase(id);

		Variant v;
		v.type = Variant::Type::UNSIGNEDINT;
		v.as_uint = id;

		Event ev;
		ev.event_type = "EntityDestroyedEvent";
		ev.arguments.push_back(v);

		Services::get().get_event_manager()->dispatch(ev);
	}
}

Entity* EntityManager::get_entity(unsigned int id)
{
	if(_entities.find(id) != _entities.end())
		return &_entities.at(id);
	else
		return nullptr;
}