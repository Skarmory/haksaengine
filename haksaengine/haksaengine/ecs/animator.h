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

		lod = 0;
		lod_interval = 0;

		accumulated_frames = 0;

		delete ptr;
	}

	float current_time;
	unsigned int accumulated_frames;
	
	unsigned int lod;
	unsigned int lod_interval;

	std::string default_animation;
	std::string current_animation;

	int animation_offset;
	int frame_offset;
};