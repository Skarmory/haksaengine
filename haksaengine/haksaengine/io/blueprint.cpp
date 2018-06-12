#include "blueprint.h"

#include "services.h"

Blueprint::~Blueprint(void)
{
	for (auto component : _components)
		delete component;
}

void Blueprint::add_component(BaseComponent* component)
{
	_components.push_back(component);
}

void Blueprint::create_new_entity(void)
{
	
}