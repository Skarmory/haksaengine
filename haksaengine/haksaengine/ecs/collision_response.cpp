#include "ecs/collision_response.h"

#include "services.h"

#include "ecs/collider.h"
#include "ecs/movement.h"

CollisionResponse::CollisionResponse(SystemOrdering order) : System(order)
{
}

void CollisionResponse::update(float delta)
{
	Terrain* terrain = Services::get().get_scene_manager()->get_terrain();

	for (auto eid : _entities)
	{
		Entity* entity = Services::get().get_entity_manager()->get_entity(eid);

		Transform* transform = entity->get_component<Transform>();
		Collider* collider = entity->get_component<Collider>();
		Movement* movement = entity->get_component<Movement>();

		movement->velocity = (_gravity * delta);

		transform->translate_by(movement->velocity);

		Ray to_ground;
		to_ground.direction = glm::vec3(0.0f, -1.0f, 0.0f);
		to_ground.position = transform->get_position();
		to_ground.position.y = 1000.0f;

		glm::vec3 xsect_point;
		terrain->intersect(to_ground, xsect_point);

		glm::mat4 aabb_transform = transform->get_transform_scale_translate();
		glm::vec3 aabb_min = aabb_transform * glm::vec4(collider->aabb.min, 1.0f);

		if (aabb_min.y < xsect_point.y)
		{
			glm::vec3 translate_by = glm::vec3(0.0f, 0.0f, 0.0f);
			translate_by.y = xsect_point.y - aabb_min.y;

			transform->translate_by(translate_by);
		}
	}
}

void CollisionResponse::on_event(Event e)
{
	if (e.event_type == "EntityCreatedEvent")
	{
		Entity* entity = Services::get().get_entity_manager()->get_entity(e.arguments[0].as_uint);

		if (entity->has_component<Collider>())
			_entities.push_back(e.arguments[0].as_uint);
	}
	else if (e.event_type == "EntityDestroyedEvent")
	{
		std::vector<unsigned int>::iterator it;

		if ((it = std::find(_entities.begin(), _entities.end(), e.arguments[0].as_uint)) != _entities.end())
		{
			std::iter_swap(it, _entities.end() - 1);
			_entities.pop_back();
		}
	}
}