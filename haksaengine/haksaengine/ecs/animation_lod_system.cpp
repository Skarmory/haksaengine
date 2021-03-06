#include "ecs/animation_lod_system.h"

#include <iostream>

#include "services.h"
#include "ecs/entity.h"

#include "ecs/animator.h"
#include "ecs/transform.h"

AnimationLodSystem::AnimationLodSystem(SystemOrdering order) : System(order)
{
	// Set up some default LoD data
	// TODO: External data should define this information
	_lod_intervals.resize(5);
	_lod_distances.resize(5);

	for (int i = 0; i < 5; i++)
	{
		_lod_intervals[i] = (i * 2);

		_lod_distances[i] = (i + 1) * (500.0f);
	}
}

void AnimationLodSystem::update(float delta)
{
	// Only update entities that are actually visible
	std::vector<unsigned int> culled_entities = Services::get<SceneManager>()->cull_by_main_camera(_entities);

	const Entity& camera = Services::get<SceneManager>()->get_main_camera();
	Transform* camera_transform = camera.get_component<Transform>();

	for (auto eid : culled_entities)
	{
		Entity* entity = Services::get<EntityManager>()->get_entity(eid);
		Transform* transform = entity->get_component<Transform>();
		Animator* animator = entity->get_component<Animator>();

		// Calculate LOD level for animation based on distance
		float distance = glm::distance(transform->get_position(), camera_transform->get_position());
		for (int lod = 0; lod < _lod_distances.size(); lod++)
		{
			animator->lod = lod;
			animator->lod_interval = _lod_intervals[lod];

			if (distance < _lod_distances[lod])
				break;
		}
	}
}

void AnimationLodSystem::on_event(Event event)
{
	if (event.event_type == "EntityCreatedEvent")
	{
		unsigned int eid = event.arguments[0].as_uint;
		Entity* entity = Services::get<EntityManager>()->get_entity(eid);

		if (entity->has_component<Animator>() && entity->has_component<Transform>())
		{
			_entities.push_back(eid);
		}
	}
	else if (event.event_type == "EntityDestroyedEvent")
	{
		unsigned int eid = event.arguments[0].as_uint;
		std::vector<unsigned int>::iterator it;
		if ((it = std::find(_entities.begin(), _entities.end(), eid)) != _entities.end())
			_entities.erase(it);
	}
}