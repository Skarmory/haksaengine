#pragma once

#include <string>

#include "ecs/component.h"
#include "anim/animation.h"

// The Animator component holds transient data for animation per-entity
struct Animator : public Component<Animator>
{
	void load(NamedVariantPack* datapack) override
	{
		std::string* ptr = static_cast<std::string*>(datapack->get("default_animation").as_ptr);
		default_animation = *ptr;
		current_animation = default_animation;
		current_time = 0.0f;

		lod_intervals.resize(5);
		lod_distances.resize(5);

		for (int i = 0; i < 5; i++)
		{
			lod_intervals[i] = (i * 2);

			lod_distances[i] = (i + 1) * (500.0f);
		}

		lod = lod_intervals[0];

		should_update = true;

		accumulated_frames = 0;

		delete ptr;
	}

	float current_time;
	unsigned int accumulated_frames;
	
	unsigned int lod;
	std::vector<float> lod_intervals;
	std::vector<float> lod_distances;

	std::string default_animation;
	std::string current_animation;
	bool should_update;
};