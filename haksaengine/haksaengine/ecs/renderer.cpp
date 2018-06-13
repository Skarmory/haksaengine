#include "ecs/renderer.h"

#include <algorithm>

#include "GL/glew.h"

#include "services.h"
#include "ecs/renderable.h"

Renderer::Renderer(void)
{
	Services::get().get_event_manager()->subscribe("AssetMeshLoaded", this);
}

void Renderer::update(float delta)
{
	for (auto entity_id : _entities)
	{
		Entity* entity = Services::get().get_entity_manager()->get_entity(entity_id);
	}
}

void Renderer::on_event(Event ev)
{
	if (ev.event_type == "EntityCreatedEvent")
	{
		unsigned int entity_id = ev.arguments[0].as_uint;
		Entity* entity = Services::get().get_entity_manager()->get_entity(entity_id);

		if (entity->has_component<Renderable>())
			_entities.push_back(entity_id);
	}
	else if (ev.event_type == "EntityDestroyedEvent")
	{
		unsigned int entity_id = ev.arguments[0].as_uint;
		std::vector<unsigned int>::iterator it;
		if ((it = std::find(_entities.begin(), _entities.end(), entity_id)) != _entities.end())
			_entities.erase(it);
	}
	else if (ev.event_type == "AssetMeshLoaded")
	{
		char c = 'v';
	}
}