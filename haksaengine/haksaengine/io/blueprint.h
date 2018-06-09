#pragma once

#include <functional>
#include <vector>

#include "ecs/entity_manager.h"
#include "ecs/component.h"
#include "ecs/entity.h"

class BaseComponent;
class EntityManager;

class Blueprint
{
public:
	void add_component(BaseComponent* component);

	void create_new_entity(void);

private:
	std::vector<BaseComponent*> _components;

	friend class EntityManager;
};