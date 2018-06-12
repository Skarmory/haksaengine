#include "ecs/entity.h"

Entity::Entity(void) {}

Entity::Entity(unsigned int id) : _id(id)
{}

Entity::~Entity(void)
{
	for (auto component_kpv : _components)
		delete component_kpv.second;
}