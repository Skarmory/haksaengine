#include "scene/scene_manager.h"

#include "services.h"
#include "ecs/camera.h"

SceneManager::SceneManager(void)
{
	Services::get<EventManager>()->subscribe("EntityCreatedEvent", this);
}

SceneManager::~SceneManager(void)
{

}

void SceneManager::on_event(Event ev)
{
	if (ev.event_type == "EntityCreatedEvent")
	{
		// Grab this if it's a camera entity
		auto entity = Services::get<EntityManager>()->get_entity(ev.arguments[0].as_uint);
		if (entity->has_component<Camera>())
		{
			main_camera = entity;
		}
	}
}

const Entity& SceneManager::get_main_camera(void) const
{
	return *main_camera;
}