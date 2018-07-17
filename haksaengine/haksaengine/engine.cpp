#include "engine.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>

#include "asset_manager.h"
#include "event/event_manager.h"
#include "ecs/entity_manager.h"
#include "ecs/transform.h"
//#include "ecs/renderer.h"
#include "ecs/renderable.h"
#include "ecs/skinned_renderable.h"
#include "ecs/camera.h"
#include "ecs/animator.h"
#include "ecs/system_ordering.h"

Engine::Engine(void) : accumulator(0.0f)
{
}

Engine::~Engine(void)
{
	delete animation_system;
	delete skinned_render_logic;
	delete game_window;
	//delete renderer;

	glfwTerminate();
}

// Callback for opengl to print errors
void gl_error_callback(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length, const GLchar* message, GLvoid* user_parameters)
{
	printf("OpenGL error: %s\n", message);
}

void Engine::initialise(void)
{
	// Initialise OpenGL stuff
	glfwInit();

	game_window = new GameWindow(800, 600, "Game Application");

	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glDebugMessageCallback((GLDEBUGPROC)gl_error_callback, nullptr);

	renderer = new Renderer;

	// Add engine services to the locator
	services.set_event_manager(new EventManager);
	services.set_entity_manager(new EntityManager);
	services.set_asset_manager(new AssetManager);
	services.set_component_manager(new ComponentManager);
	services.set_scene_manager(new SceneManager);
	services.set_renderer(renderer);

	// Create engine defined systems
	basic_render_logic = new BasicRenderSystem({ UpdatePriority::RENDER, 0 });
	skinned_render_logic = new SkinnedRenderer({ UpdatePriority::RENDER, 0 });
	animation_system = new AnimationSystem({ UpdatePriority::PRERENDER, 100 });

	// Register engine defined components
	services.get_component_manager()->register_component<Transform>("Transform");
	services.get_component_manager()->register_component<Renderable>("Renderable");
	services.get_component_manager()->register_component<SkinnedRenderable>("SkinnedRenderable");
	services.get_component_manager()->register_component<Camera>("Camera");
	services.get_component_manager()->register_component<Animator>("Animator");
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

		animation_system->update(game_time.delta());


		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		basic_render_logic->update(game_time.delta());
		skinned_render_logic->update(game_time.delta());

		renderer->render();

		game_window->swap_buffers();
	}
}