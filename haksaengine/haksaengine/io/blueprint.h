#pragma once

#include <functional>
#include <vector>

#include "asset_manager.h"
#include "ecs/entity_manager.h"
#include "ecs/component.h"
#include "ecs/entity.h"

class BaseComponent;
class EntityManager;

class Blueprint : public Asset
{
public:
	Blueprint(void) = default;
	~Blueprint(void);

	void add_component(BaseComponent* component);

	void create_new_entity(void);

private:
	std::vector<BaseComponent*> _components;

	friend class EntityManager;
};