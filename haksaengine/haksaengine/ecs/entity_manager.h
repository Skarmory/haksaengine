#pragma once

#include <vector>
#include <unordered_map>

#include "globals.h"
#include "service.h"
#include "event/event_manager.h"
#include "ecs/entity.h"
#include "ecs/component.h"
#include "io/blueprint.h"

class EventManager;
class Blueprint;

/* The EntityManager class is responsible for the lifecycle of the Entity objects. */
class EntityManager : public Service
{
public:
	EntityManager(void);
	~EntityManager(void);

	// No copying
	EntityManager(const EntityManager&) = delete;
	void operator=(const EntityManager&) = delete;

	// Creates an entity and attaches the given components to it, returns new entity's id. Sends an EntityCreatedEvent
	HAKSAENGINE_API unsigned int create_entity(std::vector<BaseComponent*>* const components = nullptr);

	// Create an entity from a blueprint, returns new entity's id. Sends an EntityCreatedEvent
	HAKSAENGINE_API unsigned int create_entity(const Blueprint* blueprint);

	// Destroy an entity by id. Sends an EntityDestroyedEvent
	HAKSAENGINE_API void destroy_entity(unsigned int id);

	// Get entity by id
	HAKSAENGINE_API Entity* get_entity(unsigned int id);

private:
	unsigned int next_id;
	std::unordered_map<unsigned int, Entity> _entities;
};
