#pragma once

#include "ecs/system.h"
#include "system_ordering.h"

// Class to set the LoD level of animated characters
class AnimationLodSystem : public System
{
public:

	AnimationLodSystem(SystemOrdering order);

	void update(float delta) override;
	void on_event(Event event) override;

private:

	std::vector<float> _lod_intervals;
	std::vector<float> _lod_distances;
};