#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ecs/component.h"
#include "scene/frustum.h"

// Represents which projection mode a camera is using
enum ProjectionMode
{
	Perspective  = 0,
	Orthographic = 1
};

// The Camera component defines the camera frustum and FoV
struct Camera : public Component<Camera>
{

public:

	// Load camera data
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

	// Sets projection mode and updates the projection matrix for this camera.
	// This method is needed because the projection matrix is constructed differently depending on whether it is perspective or orthographic
	void set_projection_mode(ProjectionMode mode)
	{
		projection_mode = mode;

		if (mode == ProjectionMode::Perspective)
			projection_matrix = glm::perspective(fov, aspect_ratio, near_plane, far_plane);
		else
			projection_matrix = glm::ortho(-(float)(width/2), (float)(width/2), -(float)(height/2), (float)(height/2), near_plane, far_plane);
	}

	// Gets projection mode
	ProjectionMode get_projection_mode(void) const
	{
		return projection_mode;
	}

	float near_plane;
	float far_plane;
	float aspect_ratio;
	float fov;
	unsigned int width;
	unsigned int height;

	glm::mat4 projection_matrix;
	glm::mat4 view_matrix;

	Frustum frustum;

private:

	ProjectionMode projection_mode;
};