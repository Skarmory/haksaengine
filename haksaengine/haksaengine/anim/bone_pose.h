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

	// Flag to set whether this node corresponds to an actual bone
	bool is_bone;

	// If is_bone is true, this indicates the bone this node is linked to
	int bone_id;

	// Animation data
	std::vector<Vector3Key> positions;
	std::vector<QuaternionKey> rotations;
	std::vector<Vector3Key> scales;

	glm::vec3 interpolate_position(float time) const
	{
		// If there is only 1 entry, this is a constant scale from time = 0 (it would not make sense for the time to be > 0)
		if (positions.size() == 1)
			return positions[0].key;

		Vector3Key left, right;

		// Go through list of translation values over time and find the entries that this time fits between
		for (int i = 0; i < positions.size(); i++)
		{
			if (positions[i].time > time)
			{
				right = positions[i];
				break;
			}

			left = positions[i];
		}

		// Normalise the time value between the two timesteps found above for interpolation
		float normalised_time = (time - left.time) / (right.time - left.time);

		// Return LERP'd value
		return glm::mix(left.key, right.key, normalised_time);
	}

	glm::quat interpolate_rotation(float time) const
	{
		// If there is only 1 entry, this is a constant scale from time = 0 (it would not make sense for the time to be > 0)
		if (rotations.size() == 1)
		{
			return rotations[0].key;
		}

		QuaternionKey left, right;

		// Go through list of rotation values over time and find the entries that this time fits between
		for (int i = 0; i < rotations.size(); i++)
		{
			if (rotations[i].time >= time)
			{
				right = rotations[i];
				break;
			}

			left = rotations[i];
		}

		// Normalise the time value between the two timesteps found above for interpolation
		float normalised_time = (time - left.time) / (right.time - left.time);

		// Return SLERP'd value
		return glm::slerp(left.key, right.key, normalised_time);
	}

	glm::vec3 interpolate_scale(float time) const
	{
		// If there is only 1 entry, this is a constant scale from time = 0 (it would not make sense for the time to be > 0)
		if (scales.size() == 1)
			return scales[0].key;

		Vector3Key left, right;

		// Go through list of scale values over time and find the entries that this time fits between
		for (int i = 0; i < scales.size(); i++)
		{
			if (scales[i].time > time)
			{
				right = scales[i];
				break;
			}

			left = scales[i];
		}

		// Normalise the time value between the two timesteps found above for interpolation
		float normalised_time = (time - left.time) / (right.time - left.time);

		// Return LERP'd value
		return glm::mix(left.key, right.key, normalised_time);
	}
};