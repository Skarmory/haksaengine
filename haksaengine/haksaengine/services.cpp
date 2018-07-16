#include "services.h"

Services* Services::services = nullptr;

Services::Services(void) : event_manager(nullptr), entity_manager(nullptr), asset_manager(nullptr)
{
	services = this;
}

Services::~Services(void)
{
	if (event_manager)
	{
		delete event_manager;
		event_manager = nullptr;
	}

	if (entity_manager)
	{
		delete entity_manager;
		entity_manager = nullptr;
	}

	if (asset_manager)
	{
		delete asset_manager;
		asset_manager = nullptr;
	}

	if (component_manager)
	{
		delete component_manager;
		component_manager = nullptr;
	}

	if (scene_manager)
	{
		delete scene_manager;
		scene_manager = nullptr;
	}

	if (renderer)
	{
		delete renderer;
		renderer = nullptr;
	}

	services = nullptr;    
}

Services& Services::get(void)
{
	return *services;
}

void Services::set_event_manager(EventManager* event_manager)
{
	this->event_manager = event_manager;
}

void Services::set_entity_manager(EntityManager* entity_manager)
{
	this->entity_manager = entity_manager;
}

void Services::set_asset_manager(AssetManager* asset_manager)
{
	this->asset_manager = asset_manager;
}

void Services::set_component_manager(ComponentManager* component_manager)
{
	this->component_manager = component_manager;
}

void Services::set_scene_manager(SceneManager* scene_manager)
{
	this->scene_manager = scene_manager;
}

void Services::set_renderer(Renderer* renderer)
{
	this->renderer = renderer;
}

EventManager* Services::get_event_manager(void)
{
	return event_manager;
}

EntityManager* Services::get_entity_manager(void)
{
	return entity_manager;
}

AssetManager* Services::get_asset_manager(void)
{
	return asset_manager;
}

ComponentManager* Services::get_component_manager(void)
{
	return component_manager;
}

SceneManager* Services::get_scene_manager(void)
{
	return scene_manager;
}

Renderer* Services::get_renderer(void)
{
	return renderer;
}