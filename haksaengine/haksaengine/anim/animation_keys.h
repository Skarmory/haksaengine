#pragma once

#include <glm/glm.hpp>

// These structs contain the animation key frame time and key frame data

struct Vector3Key
{
	float time;
	glm::vec3 key;
};

struct Vector4Key
{
	float time;
	glm::vec4 key;
};