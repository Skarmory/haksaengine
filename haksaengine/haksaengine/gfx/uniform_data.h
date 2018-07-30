#pragma once

#include <glm/glm.hpp>

#include "gfx/texture.h"

// Maximum amount of bones a model can have, corresponding to shader uniform
#define BONES_MAX 40

// Each predefined uniform block binding location
#define CAMERA_UNIFORM_BIND_POINT       0
#define PER_DRAW_UNIFORM_BIND_POINT     1
#define SCENE_UNIFORM_BIND_POINT        2
#define PER_INSTANCE_UNIFORM_BIND_POINT 3

struct CameraData
{
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 position;
};

struct PerDrawData
{
	glm::mat4    model;
	float        alpha;
	unsigned int player_colour;

	BindlessTextureHandle diffuse;
};

struct PerDrawDataSkinned
{
	glm::mat4    model;
	glm::mat4    bones[BONES_MAX];
	float        alpha;
	unsigned int player_colour;

	BindlessTextureHandle diffuse;
};

struct PerDrawDataInstancing
{
	BindlessTextureHandle diffuse;
	BindlessTextureHandle animation_data;
};

struct PerInstanceData
{
	glm::mat4 model;
	unsigned int animation_offset;
	unsigned int frame_offset;
	unsigned int player_colour;
	float alpha;
};

struct SceneData
{
	glm::vec4 sun_direction;
	glm::vec3 sun_colour;
};