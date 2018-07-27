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
		if (system->_order.timing.update_interval > 0)
		{
			system->_order.timing.accumulated_frames++;
			
			if (system->_order.timing.accumulated_frames > system->_order.timing.update_interval)
			{
				system->update(delta);
				system->_order.timing.accumulated_frames = 0;
			}

			continue;
		}

		system->update(delta);
	}
}