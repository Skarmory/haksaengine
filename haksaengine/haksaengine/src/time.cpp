#include "time.h"

GameTime::GameTime(void) : current(std::chrono::high_resolution_clock::now()), previous(std::chrono::high_resolution_clock::now())
{
}

void GameTime::tick(void)
{
	previous = current;

	current = std::chrono::high_resolution_clock::now();

	deltatime = current - previous;

	elapsedtime += deltatime;
}

float GameTime::delta(void) const
{
	return deltatime.count();
}

float GameTime::elapsed(void) const
{
	return elapsedtime.count();
}