#include "ecs/system_manager.h"

#include "services.h"

SystemManager::SystemManager(void) : _next_id(0)
{
}

SystemManager::~SystemManager(void)
{
	for (auto kpv : _systems)
	for (auto system : kpv.second)
	{
		delete system;
	}
}

bool SystemManager::_sort_systems(const System* left, const System* right)
{
	return left->_order.index < right->_order.index;
}

void SystemManager::update_systems(float delta, UpdatePriority priority)
{
	std::vector<System*>& systems = _systems[priority];

	for (auto system : systems)
	{
		system->update(delta);
	}
}