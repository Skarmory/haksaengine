#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

// These structs contain the animation key frame time and key frame data

struct Vector3Key
{
	float time;
	glm::vec3 key;
};

struct QuaternionKey
{
	float time;
	glm::quat key;
};