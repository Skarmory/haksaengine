#include "ecs/entity.h"

Entity::Entity(void) : _blueprint(0), _from_blueprint(false)
{}

Entity::Entity(unsigned int id, unsigned int blueprint) : _id(id), _blueprint(blueprint), _from_blueprint(blueprint)
{}

Entity::~Entity(void)
{
	for (auto component_kpv : _components)
		delete component_kpv.second;
}