#pragma once

#include <chrono>

#include "globals.h"
#include "service.h"

/* Class to track the elapsed time since the program started, and also
   to track delta time between frames */
class GameTime : public Service
{
public:
	GameTime(void);

	HAKSAENGINE_API float delta(void) const;
	HAKSAENGINE_API float elapsed(void) const;

	void tick(void);

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> current;
	std::chrono::time_point<std::chrono::high_resolution_clock> previous;
	std::chrono::duration<float> elapsedtime;
	std::chrono::duration<float> deltatime;
};