#include "ecs/system.h"

#include "services.h"

System::System(void)
{
	Services::get<EventManager>()->subscribe("EntityCreatedEvent", this);
	Services::get<EventManager>()->subscribe("EntityDestroyedEvent", this);
};