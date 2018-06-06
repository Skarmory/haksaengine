#pragma once

#include "globals.h"

#include "window.h"
#include "time.h"
#include "services.h"
#include "ecs/rendering_system.h"

/* Entrypoint into the engine itself. Binds everything together. */
class HAKSAENGINE_API Engine
{
public:
	Engine(void);
	~Engine(void);

	Engine(const Engine&) = delete;
	void operator=(const Engine&) = delete;

	void initialise(void);
	void run(void);

private:
	// Time stuff
	GameTime game_time;
	float accumulator;
	const float FIXED_TIME_STEP = 1.0f / 60.0f;

	GameWindow* game_window;
	Services services;
	RenderingSystem* rendering_system;
};
