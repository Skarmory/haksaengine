#include "scene/scene_manager.h"

#include "services.h"
#include "ecs/camera.h"
#include "ecs/transform.h"

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
		else if(entity->has_component<Transform>())
		{
			_transformable_entities.push_back(ev.arguments[0].as_uint);
		}
	}
	else if (ev.event_type == "EntityDestroyedEvent")
	{
		auto it = std::lower_bound(_transformable_entities.begin(), _transformable_entities.end(), ev.arguments[0].as_uint);
		
		if (it != _transformable_entities.end())
			_transformable_entities.erase(it);
	}
}

const Entity& SceneManager::get_main_camera(void) const
{
	return *main_camera;
}

void SceneManager::cull_entities(void)
{
	_culled_entities = cull_by_camera(_transformable_entities, main_camera);
	std::sort(_culled_entities.begin(), _culled_entities.end());
}

std::vector<unsigned int> SceneManager::cull_by_main_camera(const std::vector<unsigned int>& entities)
{
	std::vector<unsigned int> culled;
	culled.reserve(entities.size());

	for (auto eid : entities)
	{
		if (std::binary_search(_culled_entities.begin(), _culled_entities.end(), eid))
			culled.push_back(eid);
	}

	return culled;
}

std::vector<unsigned int> SceneManager::cull_by_camera(const std::vector<unsigned int>& entities, Entity* camera)
{
	Camera* camera_data = camera->get_component<Camera>();
	std::vector<unsigned int> ret;

	for (auto eid : entities)
	{
		Entity* entity = Services::get<EntityManager>()->get_entity(eid);

		Transform* transform = entity->get_component<Transform>();
		
		glm::vec3 pos = transform->get_position();

		if (camera_data->frustum.is_in_bounds(pos))
			ret.push_back(eid);
	}

	return ret;
}