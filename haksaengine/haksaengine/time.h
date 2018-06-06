#pragma once

#include <chrono>

#include "globals.h"

/* Class to track the elapsed time since the program started, and also
   to track delta time between frames */
class HAKSAENGINE_API GameTime
{
public:
	GameTime(void);

	void tick(void);
	float delta(void) const;
	float elapsed(void) const;

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> current;
	std::chrono::time_point<std::chrono::high_resolution_clock> previous;
	std::chrono::duration<float> elapsedtime;
	std::chrono::duration<float> deltatime;
};