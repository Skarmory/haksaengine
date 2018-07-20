#include "camera_controller.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "services.h"
#include "ecs/camera.h"
#include "ecs/transform.h"

CameraController::CameraController(SystemOrdering order) : System(order)
{
}

void CameraController::update(float delta)
{
	for (auto eid : _entities)
	{
		Entity* entity = Services::get<EntityManager>()->get_entity(eid);

		Camera* camera = entity->get_component<Camera>();
		Transform* transform = entity->get_component<Transform>();

		camera->view_matrix = glm::lookAt(transform->get_position(), transform->get_position() + transform->get_forward(), WORLD_UP);
	}
}

void CameraController::on_event(Event ev)
{
	if (ev.event_type == "EntityCreatedEvent")
	{
		unsigned int eid = ev.arguments[0].as_uint;
		Entity* entity = Services::get<EntityManager>()->get_entity(eid);

		if (entity->has_component<Camera>())
			_entities.push_back(eid);
	}
	else if (ev.event_type == "EntityDestroyedEvent")
	{
		unsigned int eid = ev.arguments[0].as_uint;
		auto it = std::find(_entities.begin(), _entities.end(), eid);

		if (it != _entities.end())
			_entities.erase(it);
	}
}