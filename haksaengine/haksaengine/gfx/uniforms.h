#pragma once

#include <glm/glm.hpp>

// Maximum amount of bones a model can have, corresponding to shader uniform
#define BONES_MAX 40

// Each predefined uniform block binding location
#define CAMERA_UNIFORM_BIND_POINT   0
#define PER_DRAW_UNIFORM_BIND_POINT 1

struct CameraUniform
{
	glm::mat4 view;
	glm::mat4 projection;
};

struct PerDrawUniform
{
	glm::mat4    model;
	glm::mat4    bones[BONES_MAX];
	float        alpha;
	unsigned int player_colour;
};