#include "unit_placer.h"

#include <glm/glm.hpp>

#include "services.h"
#include "io/blueprint.h"
#include "scene/shapes.h"
#include "ecs/transform.h"

UnitPlacer::UnitPlacer(SystemOrdering order) : System(order)
{
	Services::get().get_event_manager()->subscribe("UnitPlaceEvent", this);
	Services::get().get_event_manager()->subscribe("ObjectPlaceEvent", this);
}

void UnitPlacer::update(float delta)
{
}

void UnitPlacer::on_event(Event e)
{
	if (e.event_type == "UnitPlaceEvent")
	{
		const Blueprint* blueprint = static_cast<const Blueprint*>(e.arguments[0].as_const_ptr);

		glm::vec3 world_xsect(e.arguments[1].as_float, e.arguments[2].as_float, e.arguments[3].as_float);

		Entity& entity = Services::get().get_entity_manager()->create_and_get_entity(blueprint);

		Transform* transform = entity.get_component<Transform>();

		transform->translate(world_xsect);
	}
	else if (e.event_type == "ObjectPlaceEvent")
	{
		const Blueprint* blueprint = static_cast<const Blueprint*>(e.arguments[0].as_const_ptr);

		glm::vec3 world_xsect(e.arguments[1].as_float, e.arguments[2].as_float, e.arguments[3].as_float);

		Entity& entity = Services::get().get_entity_manager()->create_and_get_entity(blueprint);

		Transform* transform = entity.get_component<Transform>();

		transform->translate(world_xsect);
	}
}