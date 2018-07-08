#pragma once

#include <string>

#include "ecs/component.h"
#include "anim/animation.h"

struct Animator : public Component<Animator>
{
	void load(NamedVariantPack* datapack) override
	{
		std::string* ptr = static_cast<std::string*>(datapack->get("default_animation").as_ptr);
		default_animation = *ptr;
		current_animation = default_animation;
		current_time = 0.0f;

		delete ptr;
	}

	float current_time;
	std::string default_animation;
	std::string current_animation;
};