#pragma once

#include "ecs/component.h"

struct Camera : public Component<Camera>
{
	void load(NamedVariantPack* data) override
	{
		near_plane = data->get("near_plane").as_float;
		far_plane = data->get("far_plane").as_float;
		fov = data->get("fov").as_float;
	}

	float near_plane, far_plane;
	float fov;
};