#pragma once

#include <unordered_map>

#include "ecs/system.h"

class SystemManager
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

		_systems.push_back(system);

		return static_cast<SystemType*>(system);
	}

private:
	unsigned int _next_id;
	std::vector<System*> _systems;
};
