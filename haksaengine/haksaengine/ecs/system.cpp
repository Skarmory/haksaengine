#include "ecs/system.h"

#include "services.h"

System::System(SystemOrdering order) : _order(order)
{
	Services::get<EventManager>()->subscribe("EntityCreatedEvent", this);
	Services::get<EventManager>()->subscribe("EntityDestroyedEvent", this);
};