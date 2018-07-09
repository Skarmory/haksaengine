#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "anim/animation_keys.h"

// Represents a node for animation
// Some nodes don't correspond to a bone, but still contain hierarchical matrix data
struct BonePoseNode
{
	// Id and hierarchy data
	int id;
	int parent_id;
	std::vector<int> child_ids;
	glm::mat4 transform;

	// Flag to set whether this node corresponds to an actual bone
	bool is_bone;

	// If is_bone is true, this indicates the bone this node is linked to
	int bone_id;

	// Animation data
	std::vector<Vector3Key> positions;
	std::vector<QuaternionKey> rotations;
	std::vector<Vector3Key> scales;
};