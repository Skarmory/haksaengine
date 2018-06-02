#pragma once

#include "globals.h"

#include "window.h"

/* Entrypoint into the engine itself. Binds everything together. */
class HAKSAENGINE_API Engine
{
public:
	Engine(void);
	~Engine(void);

	Engine(const Engine&) = delete;
	void operator=(const Engine&) = delete;

	void initialise(void);

private:
	GameWindow* game_window;
};
