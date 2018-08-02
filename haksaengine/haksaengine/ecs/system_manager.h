#pragma once

#include <unordered_map>
#include <vector>
#include <algorithm>

#include "globals.h"
#include "service.h"
#include "ecs/system.h"

class SystemManager : public Service
{
public:
	SystemManager(void);
	~SystemManager(void);

	// No copying
	SystemManager(const SystemManager&) = delete;
	SystemManager& operator=(const SystemManager&) = delete;

	template<class SystemType, class ... Args>
	SystemType* create(Args ... args)
	{
		System* system = new SystemType(std::forward<Args ...>(args ...));

		system->_id = _next_id++;

		UpdatePriority up = system->_order.priority;

		_systems[up].push_back(system);

		std::sort(_systems[up].begin(), _systems[up].end(), _sort_systems);

		return static_cast<SystemType*>(system);
	}

	void update_systems(float delta, UpdatePriority priority);

private:
	unsigned int _next_id;
	std::unordered_map<UpdatePriority, std::vector<System*>> _systems;

	HAKSAENGINE_API static bool _sort_systems(const System* left, const System* right);
};
