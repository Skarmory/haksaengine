#pragma once

#include <functional>
#include <vector>

#include "asset_manager.h"
#include "ecs/entity_manager.h"
#include "ecs/component.h"
#include "ecs/entity.h"

class BaseComponent;
class EntityManager;

// This class contains information about pre-built entities. Complex entities with many components can easily be constructed using a blueprint.
class Blueprint : public Asset
{
public:
	Blueprint(void) = default;
	~Blueprint(void);

	// Add component to the blueprint. This component will be copied when constructing an entity from this blueprint.
	void add_component(BaseComponent* component);

private:
	std::vector<BaseComponent*> _components;

	friend class EntityManager;
};