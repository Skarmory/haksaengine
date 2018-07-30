#pragma once

#include <vector>

#include "anim/animation.h"
#include "anim/animation_keys.h"

enum class InterpolationMode;

// Holds data to do with mesh data over time
// Currently, only supports alpha changing over time
struct GeosetAnim
{
	unsigned int geoset_id;
	InterpolationMode interp_mode;
	std::vector<FloatKey> alphas;
};
