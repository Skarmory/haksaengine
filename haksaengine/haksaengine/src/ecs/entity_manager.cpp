#include "ecs/entity_manager.h"

EntityManager::EntityManager(EventManager* event_manager) : next_id(0), event_manager(event_manager)
{
}

EntityManager::~EntityManager(void)
{
}

unsigned int EntityManager::create_entity(std::vector<Component*>* const components)
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

	event_manager->dispatch(ev);

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

		event_manager->dispatch(ev);
	}
}

Entity* EntityManager::get_entity(unsigned int id)
{
	if(_entities.find(id) != _entities.end())
		return &_entities.at(id);
	else
		return nullptr;
}