#pragma once

#include <string>
#include <glm/glm.hpp>

// Struct to hold the data directly relevant to a bone in skeletal animation
struct Bone
{
	// Index of bone in the vector
	int bone_id;

	// Inverse bind matrix of the bone
	// Undoes the bind pose transformation applied to vertices affected by this bone
	glm::mat4 inverse_bind;
};
