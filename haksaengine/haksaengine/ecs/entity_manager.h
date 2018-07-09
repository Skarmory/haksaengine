#pragma once

#include <vector>
#include <unordered_map>

#include "globals.h"
#include "event/event_manager.h"
#include "ecs/entity.h"
#include "ecs/component.h"
#include "io/blueprint.h"

class EventManager;
class Blueprint;

/* The EntityManager class is responsible for the lifecycle of the Entity objects. */
class EntityManager
{
public:
	EntityManager(void);
	~EntityManager(void);

	EntityManager(const EntityManager&) = delete;
	void operator=(const EntityManager&) = delete;

	HAKSAENGINE_API unsigned int create_entity(std::vector<BaseComponent*>* const components = nullptr);

	HAKSAENGINE_API unsigned int create_entity(const Blueprint* blueprint);

	HAKSAENGINE_API void destroy_entity(unsigned int id);

	HAKSAENGINE_API Entity* get_entity(unsigned int id);

private:
	unsigned int next_id;
	std::unordered_map<unsigned int, Entity> _entities;
};
