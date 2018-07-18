#pragma once

// Where abouts in the update pipeline this system should be updated
enum UpdatePriority
{
	PRERENDER  = 0,
	RENDER     = 1,
	POSTRENDER = 2
};

// Represents the ordering of system deriving classes' updating.
// Systems with a lower index will be updated before systems with a higher index
struct SystemOrdering
{
	SystemOrdering(UpdatePriority priority, unsigned int index) : priority(priority), index(index)
	{
	}

	UpdatePriority priority;
	unsigned int index;
};