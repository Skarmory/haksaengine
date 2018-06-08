#pragma once

#include "globals.h"

#include "window.h"
#include "time.h"
#include "services.h"
#include "ecs/rendering_system.h"

/* Entrypoint into the engine itself. Binds everything together. */
class Engine
{
public:
	HAKSAENGINE_API Engine(void);
	HAKSAENGINE_API ~Engine(void);

	Engine(const Engine&) = delete;
	void operator=(const Engine&) = delete;

	HAKSAENGINE_API void initialise(void);
	HAKSAENGINE_API void run(void);

private:
	// Time stuff
	GameTime game_time;
	float accumulator;
	const float FIXED_TIME_STEP = 1.0f / 60.0f;

	GameWindow* game_window;
	Services services;
	RenderingSystem* rendering_system;
};
