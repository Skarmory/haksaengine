#include "engine.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>

#include "asset_manager.h"
#include "event/event_manager.h"
#include "ecs/entity_manager.h"
#include "ecs/transform.h"
#include "ecs/renderer.h"

Engine::Engine(void) : accumulator(0.0f)
{
}

Engine::~Engine(void)
{
	delete rendering_system;
	delete game_window;

	glfwTerminate();
}

void Engine::initialise(void)
{
	glfwInit();

	game_window = new GameWindow(800, 600, "Game Application");

	glewExperimental = GL_TRUE;
	glewInit();

	services.set_event_manager(new EventManager);
	services.set_entity_manager(new EntityManager);
	services.set_asset_manager(new AssetManager);
	services.set_component_manager(new ComponentManager);

	rendering_system = new RenderingSystem;

	services.get_component_manager()->register_component<Transform>("Transform");
	services.get_component_manager()->register_component<Renderer>("Renderer");
}

void Engine::run(void)
{
	while (!game_window->window_close())
	{
		game_time.tick();
		accumulator += game_time.delta();

		while(accumulator >= FIXED_TIME_STEP)
		{
			glfwPollEvents();

			accumulator -= FIXED_TIME_STEP;
		}

		std::cout << game_time.delta() << std::endl;

		rendering_system->update(game_time.delta());
	}
}