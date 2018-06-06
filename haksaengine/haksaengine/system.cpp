#include "ecs/system.h"

#include "services.h"

System::System(void)
{
	Services::get().get_event_manager()->subscribe("EntityCreatedEvent", this);
	Services::get().get_event_manager()->subscribe("EntityDestroyedEvent", this);
};