#pragma once

#include <vector>

#include "anim/animation.h"
#include "anim/animation_keys.h"

enum InterpolationMode;

struct GeosetAnim
{
	unsigned int geoset_id;
	InterpolationMode interp_mode;
	std::vector<FloatKey> alphas;
};
