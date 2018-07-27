#pragma once

// Where abouts in the update pipeline this system should be updated
enum UpdatePriority
{
	POSTINPUT  = 0,
	GAMEPLAY   = 1,
	PRERENDER  = 2,
	RENDER     = 3,
	POSTRENDER = 4
};

struct UpdateTiming
{
	UpdateTiming(unsigned int interval) : accumulated_frames(0), update_interval(interval)
	{
	}

	unsigned int accumulated_frames;
	unsigned int update_interval;
};

// Represents the ordering of system deriving classes' updating.
// Systems with a lower index will be updated before systems with a higher index
struct SystemOrdering
{
	SystemOrdering(UpdateTiming timing, UpdatePriority priority, unsigned int index) : timing(timing), priority(priority), index(index)
	{
	}

	UpdateTiming timing;
	UpdatePriority priority;
	unsigned int index;
};