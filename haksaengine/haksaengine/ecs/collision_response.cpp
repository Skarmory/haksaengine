#include "ecs/collision_response.h"

#include "services.h"

#include "ecs/collider.h"
#include "ecs/movement.h"

CollisionResponse::CollisionResponse(SystemOrdering order) : System(order)
{
}

void CollisionResponse::update(float delta)
{
	// This system is very simple right now, and is not designed as a fully functional physics sim.
	// Apply some gravity to entities that can move and check for terrain intersection
	Terrain* terrain = Services::get().get_scene_manager()->get_terrain();

	for (auto eid : _entities)
	{
		Entity* entity = Services::get().get_entity_manager()->get_entity(eid);

		Transform* transform = entity->get_component<Transform>();
		Collider* collider = entity->get_component<Collider>();
		Movement* movement = entity->get_component<Movement>();

		if (movement)
		{
			// Add gravity
			movement->velocity += (_gravity * delta);

			transform->translate_by(movement->velocity);
		}

		Ray to_ground;
		to_ground.direction = glm::vec3(0.0f, -1.0f, 0.0f);
		to_ground.position = transform->get_position();
		to_ground.position.y = 1000.0f;

		// Since this is primarily an RTS engine, stop entities from falling through the terrain
		// Fire a ray at the ground from the entity's x,z position on the map and see where it intersects
		glm::vec3 xsect_point;
		if (terrain->intersect(to_ground, xsect_point)) // Is entity over the terrain at all?
		{
			if (movement)
			{
				// Cancel out the y velocity of this object now to stop it from accumulating further
				movement->velocity.y += -movement->velocity.y;
			}

			glm::mat4 aabb_transform = transform->get_transform_scale_translate();
			glm::vec3 aabb_min = aabb_transform * glm::vec4(collider->aabb.min, 1.0f);

			// If AABB minimum y is less than ray terrain intersection point, the entity is clipping the terrain
			if (aabb_min.y < xsect_point.y)
			{
				glm::vec3 translate_by = glm::vec3(0.0f, 0.0f, 0.0f);
				translate_by.y = xsect_point.y - aabb_min.y;

				transform->translate_by(translate_by);
			}
		}

		// Kill-plane for entities that fall too far downwards. If they go this far down then they've fallen
		// through the level most likely. Destroy them.
		if (transform->get_position().y < -10000.0f)
			Services::get().get_entity_manager()->destroy_entity(eid);
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