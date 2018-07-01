#pragma once

#include <vector>
#include <string>

#include "anim/bone_pose.h"

// This struct contains all the information relevant to animation. 
// Every animation update, this data must be traversed and the final transforms of each bone worked out for every object with a given animation.
struct Animation
{
	// Animation name
	std::string name;

	// Duration of animation in seconds
	float duration;

	// These contain the actual translation, rotation, and scaling information for each node in the animation hierarchy over time
	BonePoseNode* root_pose_node;
	std::vector<BonePoseNode> pose_nodes;
};
