#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "ecs/system.h"
#include "anim/bone.h"
#include "anim/bone_pose.h"

// This is an engine system that processes the skeletal animation data each time it needs updating.
// It interpolates between poses of the currently playing animation, and stores them in the SkinnedRenderable of the entity.
class AnimationSystem : public System
{
public:

	AnimationSystem(void);
	virtual void update(float delta) override;
	virtual void on_event(Event ev) override;

private:

	// Descend the animation pose node tree and set the final transform for each bone
	void _process_bone_hierarchy(const BonePoseNode* node, float time, const std::vector<BonePoseNode>& nodes, const std::vector<Bone>& bones, std::vector<glm::mat4>& final_transforms, glm::mat4 parent_transform);

	// Interpolate the scale values for an animation pose node at a given time
	glm::vec3 _interpolate_scale(float time, const BonePoseNode* pose);

	// Interpolate the rotation quaternion for an animation pose node at a given time
	glm::quat _interpolate_rotation(float time, const BonePoseNode* pose);

	// Interpolates to translation values for an animation pose node at a given time
	glm::vec3 _interpolate_position(float time, const BonePoseNode* pose);
};
