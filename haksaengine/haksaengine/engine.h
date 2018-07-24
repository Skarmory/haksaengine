#pragma once

#include "globals.h"

#include "glfw_window.h"
#include "game_time.h"
#include "services.h"
#include "gfx/renderer.h"
#include "ecs/basic_render_system.h"
#include "ecs/skinned_renderer.h"
#include "ecs/animation_system.h"

// Tells the engine how to initialise itself
// Game mode will init and create a GLFW window
// Editor mode doesn't init GLFW nor create a window
enum class EngineMode
{
	Game   = 0,
	Editor = 1
};

// Represents what the engine is currently doing
enum class EngineState
{
	Uninitialised   = 0,
	Ready           = 1,
	ProcessingFrame = 2
};

/* Entrypoint into the engine itself. Binds everything together. */
class Engine
{
public:
	HAKSAENGINE_API Engine(EngineMode mode);
	HAKSAENGINE_API ~Engine(void);

	Engine(const Engine&) = delete;
	void operator=(const Engine&) = delete;

	HAKSAENGINE_API void initialise(void);

	// Process one frame of updates and rendering
	HAKSAENGINE_API void one_frame(void);

	// Runs the engine main loop, handing off control of the program until the engine terminates
	HAKSAENGINE_API void run(void);

	HAKSAENGINE_API EngineState get_state(void) const;

private:
	// Time stuff
	float accumulator;
	const float FIXED_TIME_STEP = 1.0f / 60.0f;

	GlfwWindow* game_window;
	Services services;

	EngineMode _mode;
	EngineState _state;
};
