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