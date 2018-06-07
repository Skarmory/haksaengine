#pragma once

#include "globals.h"
#include "event/event_manager.h"
#include "ecs/entity_manager.h"
#include "asset_manager.h"

/* Class that acts as an access point for all manager classes */
class HAKSAENGINE_API Services
{
public:
	Services(void);
	Services(const Services&) = delete;
	void operator=(const Services&) = delete;

	~Services(void);

	static Services& get(void);

	void set_event_manager(EventManager* event_manager);
	void set_entity_manager(EntityManager* entity_manager);
	void set_asset_manager(AssetManager* asset_manager);

	EventManager* get_event_manager(void);
	EntityManager* get_entity_manager(void);
	AssetManager* get_asset_manager(void);

private:
	static Services* services;

	EventManager* event_manager;
	EntityManager* entity_manager;
	AssetManager* asset_manager;
};
