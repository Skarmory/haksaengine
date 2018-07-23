#include "engine.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>

#include "asset_manager.h"
#include "event/event_manager.h"
#include "ecs/entity_manager.h"
#include "ecs/transform.h"
#include "ecs/renderable.h"
#include "ecs/skinned_renderable.h"
#include "ecs/camera.h"
#include "ecs/animator.h"
#include "ecs/system_ordering.h"
#include "ecs/system_manager.h"
#include "ecs/camera_controller.h"

Engine::Engine(EngineMode mode) : accumulator(0.0f), _mode(mode), _state(EngineState::Ready)
{
}

Engine::~Engine(void)
{
	if (_mode == EngineMode::Game)
	{
		delete game_window;

		glfwTerminate();
	}
}

// Callback for opengl to print errors
void gl_error_callback(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length, const GLchar* message, GLvoid* user_parameters)
{
	printf("OpenGL error: %s\n", message);
}

void Engine::initialise(void)
{
	if (_mode == EngineMode::Game)
	{
		// Initialise OpenGL stuff
		glfwInit();

		game_window = new GameWindow(800, 600, "Game Application");
	}

	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glDebugMessageCallback((GLDEBUGPROC)gl_error_callback, nullptr);

	Renderer* renderer = new Renderer;
	GameTime* game_time = new GameTime;
	ComponentManager* compman = new ComponentManager;
	SystemManager* sysman = new SystemManager;

	// Add engine services to the locator
	services.set_event_manager(new EventManager);
	services.set_entity_manager(new EntityManager);
	services.set_asset_manager(new AssetManager);
	services.set_component_manager(compman);
	services.set_scene_manager(new SceneManager);
	services.set_renderer(renderer);
	services.set_system_manager(sysman);
	services.set_game_time(game_time);

	// Create engine defined systems
	sysman->create<SkinnedRenderer>(SystemOrdering(UpdatePriority::RENDER, 1));
	sysman->create<BasicRenderSystem>(SystemOrdering(UpdatePriority::RENDER, 0));
	sysman->create<AnimationSystem>(SystemOrdering(UpdatePriority::PRERENDER, 100));
	sysman->create<CameraController>(SystemOrdering(UpdatePriority::POSTINPUT, 0));

	// Register engine defined components
	compman->register_component<Transform>("Transform");
	compman->register_component<Renderable>("Renderable");
	compman->register_component<SkinnedRenderable>("SkinnedRenderable");
	compman->register_component<Camera>("Camera");
	compman->register_component<Animator>("Animator");
}

void Engine::one_frame(void)
{
	// Engine is already processing a frame, so just return without doing anything
	if (_state != EngineState::Ready)
		return;

	_state = EngineState::ProcessingFrame;

	SystemManager* sysman = services.get_system_manager();
	SceneManager* sceneman = services.get_scene_manager();
	Renderer* renderer = services.get_renderer();
	GameTime* time = services.get_game_time();

	float delta;

	time->tick();
	delta = time->delta();
	accumulator += delta;

	while (accumulator >= FIXED_TIME_STEP)
	{
		glfwPollEvents();

		accumulator -= FIXED_TIME_STEP;
	}

	//std::cout << game_time.delta() << std::endl;

	sysman->update_systems(delta, UpdatePriority::POSTINPUT);

	sceneman->cull_entities();

	sysman->update_systems(delta, UpdatePriority::PRERENDER);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sysman->update_systems(delta, UpdatePriority::RENDER);

	renderer->render();

	sysman->update_systems(delta, UpdatePriority::POSTRENDER);

	game_window->swap_buffers();

	_state = EngineState::Ready;
}

void Engine::run(void)
{
	while (!game_window->window_close())
	{
		one_frame();
	}
}

EngineState Engine::get_state(void) const
{
	return _state;
}