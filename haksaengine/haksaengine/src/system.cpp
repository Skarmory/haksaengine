#include "ecs/system.h"

System::System(EntityManager* entity_manager, EventManager* event_manager) : entity_manager(entity_manager), event_manager(event_manager)
{
	event_manager->subscribe("EntityCreatedEvent", this);
	event_manager->subscribe("EntityDestroyedEvent", this);
};