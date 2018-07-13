#pragma once

#include <typeindex>

#include "globals.h"
#include "event/event_manager.h"
#include "ecs/entity_manager.h"
#include "ecs/component_manager.h"
#include "asset_manager.h"
#include "scene/scene_manager.h"

/* Class that acts as an access point for all manager classes */
class Services
{
public:
	Services(void);
	Services(const Services&) = delete;
	void operator=(const Services&) = delete;

	~Services(void);

	// Get the Services instance
	HAKSAENGINE_API static Services& get(void);

	// Gets service by templated service type.
	// If ServiceType is not a Service derived class or the service is not initialised, return nullptr
	template<class ServiceType>
	static ServiceType* get(void)
	{
		std::type_index type = typeid(ServiceType);
		Service* service;

		if (type == typeid(EventManager))
			service = get().get_event_manager();

		else if (type == typeid(EntityManager))
			service = get().get_entity_manager();

		else if (type == typeid(AssetManager))
			service = get().get_asset_manager();

		else if (type == typeid(ComponentManager))
			service = get().get_component_manager();

		else if (type == typeid(SceneManager))
			service = get().get_scene_manager();
		else
			return nullptr;

		return static_cast<ServiceType*>(service);
	}

	// Add service methods
	void set_event_manager(EventManager* event_manager);
	void set_entity_manager(EntityManager* entity_manager);
	void set_asset_manager(AssetManager* asset_manager);
	void set_component_manager(ComponentManager* component_manager);
	void set_scene_manager(SceneManager* scene_manager);

	// Get service methods
	HAKSAENGINE_API EventManager* get_event_manager(void);
	HAKSAENGINE_API EntityManager* get_entity_manager(void);
	HAKSAENGINE_API AssetManager* get_asset_manager(void);
	HAKSAENGINE_API ComponentManager* get_component_manager(void);
	HAKSAENGINE_API SceneManager* get_scene_manager(void);

private:
	static Services* services;

	EventManager* event_manager;
	EntityManager* entity_manager;
	AssetManager* asset_manager;
	ComponentManager* component_manager;
	SceneManager* scene_manager;
};
