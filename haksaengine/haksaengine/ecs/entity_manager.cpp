#include "ecs/entity_manager.h"

#include <iostream>

#include "services.h"
#include "variant.h"


EntityManager::EntityManager(void) : next_id(0)
{
}

EntityManager::~EntityManager(void)
{
}

unsigned int EntityManager::create_entity(std::vector<BaseComponent*>* const components)
{
	_entities[next_id] = Entity(next_id, 0);

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

	Services::get<EventManager>()->dispatch(ev);

	return next_id++;
}

unsigned int EntityManager::create_entity(const Blueprint* blueprint)
{
	_entities[next_id] = Entity(next_id, blueprint->get_id());

	for (auto component : blueprint->_components)
	{
		_entities[next_id].add_component(component->clone());
	}

	Variant v;
	v.type = Variant::Type::UNSIGNEDINT;
	v.as_uint = next_id;

	Event ev;
	ev.event_type = "EntityCreatedEvent";
	ev.arguments.push_back(v);

	Services::get<EventManager>()->dispatch(ev);

	return next_id++;
}

Entity& EntityManager::create_and_get_entity(const Blueprint* blueprint)
{
	return *get_entity(create_entity(blueprint));
}

void EntityManager::destroy_entity(unsigned int id, bool immediate)
{
	if (!immediate)
	{
		// Set the entity to be obsolete so it can be destroyed at a more convenient time
		if (_entities.find(id) != _entities.end())
		{
			_entities.at(id)._obsolete = true;
			_obsolete.push_back(id);
		}
	}
	else
	{
		_destroy_entity(id);
	}
}

Entity* EntityManager::get_entity(unsigned int id)
{
	if(_entities.find(id) != _entities.end())
		return &_entities.at(id);
	else
		return nullptr;
}

void EntityManager::update(void)
{
	if (!_obsolete.empty())
	{
		for (int i = 0; i < _obsolete.size(); i++)
		{
			_destroy_entity(_obsolete[i]);
		}

		_obsolete.clear();
	}
}

void EntityManager::_destroy_entity(unsigned int id)
{
	_entities.erase(id);

	Variant v;
	v.type = Variant::Type::UNSIGNEDINT;
	v.as_uint = id;

	Event ev;
	ev.event_type = "EntityDestroyedEvent";
	ev.arguments.push_back(v);

	Services::get<EventManager>()->dispatch(ev);
}