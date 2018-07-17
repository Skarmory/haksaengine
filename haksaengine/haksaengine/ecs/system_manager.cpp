#include "ecs/system_manager.h"

SystemManager::SystemManager(void) : _next_id(0)
{
}

SystemManager::~SystemManager(void)
{
	for (auto system : _systems)
		delete system;
}