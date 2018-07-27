#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "ecs/system.h"
#include "anim/bone.h"
#include "anim/bone_pose.h"
#include "anim/geoset_anim.h"

// This is an engine system that processes the skeletal animation data each time it needs updating.
// It interpolates between poses of the currently playing animation, and stores them in the SkinnedRenderable of the entity.
class AnimationSystem : public System
{
public:

	AnimationSystem(SystemOrdering order);
	virtual void update(float delta) override;
	virtual void on_event(Event ev) override;

private:

	// Threaded update
	void _do_animator_update(int start, int count, float delta);
	void _do_animation_update(int start, int count, const std::vector<unsigned int>& culled_entities, float delta);

	// Descend the animation pose node tree and set the final transform for each bone
	void _process_bone_hierarchy(const BonePoseNode* node, float time, const std::vector<BonePoseNode>& nodes, const std::vector<Bone>& bones, std::vector<glm::mat4>& final_transforms, glm::mat4 parent_transform);

	// Set the alpha value of each mesh at current time
	void _process_geoset_anims(float time, const std::vector<GeosetAnim>& geoset_anims, std::vector<float>& alpha_values);

	// Interpolate the scale values for an animation pose node at a given time
	glm::vec3 _interpolate_scale(float time, const BonePoseNode* pose);

	// Interpolate the rotation quaternion for an animation pose node at a given time
	glm::quat _interpolate_rotation(float time, const BonePoseNode* pose);

	// Interpolates to translation values for an animation pose node at a given time
	glm::vec3 _interpolate_position(float time, const BonePoseNode* pose);

	// Interpolate the geoset alpha at a given time
	float _interpolate_geoset_alpha(float time, const GeosetAnim* anim);
};
