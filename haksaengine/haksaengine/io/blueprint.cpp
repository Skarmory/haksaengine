#include "blueprint.h"

#include "services.h"

void Blueprint::add_component(BaseComponent* component)
{
	_components.push_back(component);
}

void Blueprint::create_new_entity(void)
{
	
}