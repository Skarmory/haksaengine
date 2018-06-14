#include "engine.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>

#include "asset_manager.h"
#include "event/event_manager.h"
#include "ecs/entity_manager.h"
#include "ecs/transform.h"
#include "ecs/renderer.h"
#include "ecs/renderable.h"

Engine::Engine(void) : accumulator(0.0f)
{
}

Engine::~Engine(void)
{
	delete renderer;
	delete game_window;

	glfwTerminate();
}

void gl_error_callback(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length, const GLchar* message, GLvoid* user_parameters)
{
	printf("OpenGL error: %s\n", message);
}

void Engine::initialise(void)
{
	glfwInit();

	game_window = new GameWindow(800, 600, "Game Application");

	glewExperimental = GL_TRUE;
	glewInit();

	glDebugMessageCallback((GLDEBUGPROC)gl_error_callback, nullptr);

	services.set_event_manager(new EventManager);
	services.set_entity_manager(new EntityManager);
	services.set_asset_manager(new AssetManager);
	services.set_component_manager(new ComponentManager);

	renderer = new Renderer;

	services.get_component_manager()->register_component<Transform>("Transform");
	services.get_component_manager()->register_component<Renderable>("Renderable");
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

		renderer->update(game_time.delta());
	}
}