#pragma once

#include <vector>
#include <unordered_map>

#include "globals.h"
#include "event_manager.h"
#include "ecs/entity.h"
#include "ecs/component.h"

class EventManager;

/* The EntityManager class is responsible for the lifecycle of the Entity objects. */
class HAKSAENGINE_API EntityManager
{
public:
	EntityManager(EventManager* event_manager);
	~EntityManager(void);

	EntityManager(const EntityManager&) = delete;
	void operator=(const EntityManager&) = delete;

	unsigned int create_entity(std::vector<Component*>* const components = nullptr);

	void destroy_entity(unsigned int id);

	Entity* get_entity(unsigned int id);

private:
	unsigned int next_id;
	std::unordered_map<unsigned int, Entity> _entities;
	EventManager* event_manager;
};
