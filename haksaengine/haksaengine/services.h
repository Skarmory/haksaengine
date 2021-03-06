#pragma once

#include <typeindex>

#include "globals.h"
#include "event/event_manager.h"
#include "ecs/entity_manager.h"
#include "ecs/component_manager.h"
#include "ecs/system_manager.h"
#include "asset_manager.h"
#include "scene/scene_manager.h"
#include "gfx/renderer.h"
#include "game_time.h"
#include "input/input_manager.h"

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

		else if (type == typeid(SystemManager))
			service = get().get_system_manager();

		else if (type == typeid(GameTime))
			service = get().get_game_time();

		else if (type == typeid(Renderer))
			service = get().get_renderer();

		else if (type == typeid(InputManager))
			service = get().get_input_manager();

		else
			return nullptr;

		return static_cast<ServiceType*>(service);
	}

	// Add service methods
	HAKSAENGINE_API void set_event_manager(EventManager* event_manager);
	HAKSAENGINE_API void set_entity_manager(EntityManager* entity_manager);
	HAKSAENGINE_API void set_asset_manager(AssetManager* asset_manager);
	HAKSAENGINE_API void set_component_manager(ComponentManager* component_manager);
	HAKSAENGINE_API void set_scene_manager(SceneManager* scene_manager);
	HAKSAENGINE_API void set_system_manager(SystemManager* system_manager);
	HAKSAENGINE_API void set_game_time(GameTime* renderer);
	HAKSAENGINE_API void set_renderer(Renderer* renderer);
	HAKSAENGINE_API void set_input_manager(InputManager* input_manager);

	// Get service methods
	HAKSAENGINE_API EventManager* get_event_manager(void);
	HAKSAENGINE_API EntityManager* get_entity_manager(void);
	HAKSAENGINE_API AssetManager* get_asset_manager(void);
	HAKSAENGINE_API ComponentManager* get_component_manager(void);
	HAKSAENGINE_API SceneManager* get_scene_manager(void);
	HAKSAENGINE_API SystemManager* get_system_manager(void);
	HAKSAENGINE_API GameTime* get_game_time(void);
	HAKSAENGINE_API Renderer* get_renderer(void);
	HAKSAENGINE_API InputManager* get_input_manager(void);

private:
	static Services* services;

	EventManager* event_manager;
	EntityManager* entity_manager;
	AssetManager* asset_manager;
	ComponentManager* component_manager;
	SceneManager* scene_manager;
	SystemManager* system_manager;
	GameTime* game_time;
	Renderer* renderer;
	InputManager* input_manager;
};
