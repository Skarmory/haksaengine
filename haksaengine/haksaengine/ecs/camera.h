#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ecs/component.h"

enum ProjectionMode
{
	Perspective  = 0,
	Orthographic = 1
};

// The Camera component defines the camera frustum and FoV
struct Camera : public Component<Camera>
{
	void load(NamedVariantPack* data) override
	{
		near_plane = data->get("near_plane").as_float;
		far_plane = data->get("far_plane").as_float;
		fov = data->get("fov").as_float;
		width = data->get("width").as_uint;
		height = data->get("height").as_uint;

		aspect_ratio = (float)width / (float)height;

		set_projection_mode(static_cast<ProjectionMode>(data->get("projection_mode").as_uint));
	}

	void set_projection_mode(ProjectionMode mode)
	{
		projection_mode = mode;

		if (mode == ProjectionMode::Perspective)
			projection_matrix = glm::perspective(fov, aspect_ratio, near_plane, far_plane);
		else
			projection_matrix = glm::ortho(0.0f, (float)width, 0.0f, (float)height, near_plane, far_plane);
	}

	float near_plane, far_plane;
	float fov;
	unsigned int width;
	unsigned int height;
	float aspect_ratio;
	ProjectionMode projection_mode;

	glm::mat4 projection_matrix;
	glm::mat4 view_matrix;
};