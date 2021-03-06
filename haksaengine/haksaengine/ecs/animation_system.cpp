#include "ecs/animation_system.h"

#include <iostream>
#include <thread>

#include "services.h"
#include "ecs/transform.h"
#include "ecs/animator.h"
#include "ecs/skinned_renderable.h"
#include "io/mdl.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

AnimationSystem::AnimationSystem(SystemOrdering order) : System(order)
{
}

void AnimationSystem::update(float delta)
{
	std::vector<unsigned int> culled_entities = Services::get<SceneManager>()->cull_by_main_camera(_entities);

	_do_animator_update(delta);
	_do_animation_update(culled_entities, delta);
}

void AnimationSystem::on_event(Event ev)
{
	if (ev.event_type == "EntityCreatedEvent")
	{
		unsigned int entity_id = ev.arguments[0].as_uint;
		Entity* entity = Services::get<EntityManager>()->get_entity(entity_id);

		if (entity->has_component<Animator>() && entity->has_component<SkinnedRenderable>())
		{
			// Resize the final bone transforms vector to the correct size for direct indexing in animation logic
			SkinnedRenderable* renderable = entity->get_component<SkinnedRenderable>();
			MDLFile& mdl = Services::get<AssetManager>()->get_asset<MDLFile>(renderable->model);

			renderable->final_bone_transforms.resize(mdl.get_bones().size());
			renderable->geoset_alphas.resize(mdl.get_meshes().size());

			_entities.push_back(entity_id);
		}
	}
	else if (ev.event_type == "EntityDestroyedEvent")
	{
		unsigned int entity_id = ev.arguments[0].as_uint;
		std::vector<unsigned int>::iterator it;
		if ((it = std::find(_entities.begin(), _entities.end(), entity_id)) != _entities.end())
		{
			std::iter_swap(it, _entities.end() - 1);
			_entities.pop_back();
		}
	}
}

void AnimationSystem::_do_animator_update(float delta)
{
	for(auto eid : _entities)
	{
		Entity* entity = Services::get<EntityManager>()->get_entity(eid);

		Animator* animator = entity->get_component<Animator>();
		SkinnedRenderable* renderable = entity->get_component<SkinnedRenderable>();

		// Update current animation time
		animator->current_time += delta;

		MDLFile& data = Services::get<AssetManager>()->get_asset<MDLFile>(renderable->model);
		const Animation* animation = data.get_animation(animator->current_animation);

		// Do animation looping by modulo of current time with animation duration
		if (animator->current_time > animation->duration)
		{
			animator->current_time = std::fmod(animator->current_time, animation->duration);
		}
	}
}

void AnimationSystem::_do_animation_update(const std::vector<unsigned int>& culled_entities, float delta)
{
	for(auto eid : culled_entities)
	{
		Entity* entity = Services::get<EntityManager>()->get_entity(eid);

		Animator* animator = entity->get_component<Animator>();
		SkinnedRenderable* renderable = entity->get_component<SkinnedRenderable>();

		animator->accumulated_frames++;

		// Check whether to update the animation based on its current LOD setting
		if (animator->accumulated_frames >= animator->lod_interval)
		{
			animator->accumulated_frames = 0;
		}
		else
		{
			continue;
		}

		MDLFile& data = Services::get<AssetManager>()->get_asset<MDLFile>(renderable->model);
		const Animation* animation = data.get_animation(animator->current_animation);

		const Texture* tex = data.get_animation_texture();

		// If this animation uses a GPU animation data texture
		if (tex)
		{
			// Figure out the offset of the animation in the animation texture
			// Also figure out the offset of the frame in the current animation
			int current_animation_id = data.get_animation_id(animation->name);
			int current_frame_id     = std::floorf(animator->current_time / (1.0f / 60.0f));

			const std::vector<Animation>& animations = data.get_animations();

			int accumulated_anim_offset = 0;

			for (int anim_id = 0; anim_id < current_animation_id; anim_id++)
			{
				accumulated_anim_offset += (animations[anim_id].frame_count);
			}

			// Each texel in the texture is a column of a bone transform matrix (4x4 matrix)
			// Each frame consists of n bone transform matrices
			// Each animation consists of x frames
			// These calculations get the first frame of an animation (the start of the animation) and the first bone matrix of the current frame as an offset from the start of the animation
			animator->animation_offset = 4 * data.get_bones().size() * accumulated_anim_offset;
			animator->frame_offset     = 4 * data.get_bones().size() * current_frame_id;
		}
		else
		{
			// Do CPU side animation interpolation and matrix hierarchy multiplication
			// This is less efficient computationally, but more efficient spatially
			// Shouldn't be used on mass scale
			_process_bone_hierarchy(animation->root_pose_node, animator->current_time, animation->pose_nodes, data.get_bones(), renderable->final_bone_transforms, glm::mat4(1.0f));
		}

		// Animate geometry data (alpha value)
		_process_geoset_anims(animator->current_time, animation->geoset_anims, renderable->geoset_alphas);
	}
}

void AnimationSystem::_process_bone_hierarchy(const BonePoseNode* node, float time, const std::vector<BonePoseNode>& nodes, const std::vector<Bone>& bones, std::vector<glm::mat4>& final_transforms, glm::mat4 parent_transform)
{
	// Get the correct animation values via interpolation
	glm::vec3 scale = node->interpolate_scale(time);
	glm::quat rotation = node->interpolate_rotation(time);
	glm::vec3 position = node->interpolate_position(time);

	// Compose the values into matrices
	glm::mat4 scalem = glm::scale(glm::mat4(1.0f), scale);
	glm::mat4 rotm = glm::mat4_cast(rotation);
	glm::mat4 transm = glm::translate(glm::mat4(1.0f), position);

	// Local transform matrix for this pose node at this time
	glm::mat4 transform = transm * rotm * scalem;

	// Multiply by parent to get global transform of this pose node at this time
	transform = parent_transform * transform;

	// If this pose node corresponds to an actual bone, set the matrix in the list of final bone transforms
	if (node->is_bone)
	{
		const Bone& bone = bones[node->bone_id];
		
		final_transforms[node->bone_id] = transform * glm::transpose(bone.inverse_bind);
	}
	
	// Go through hierarchy by sending this node's transform matrix to its children
	for (auto child : node->child_ids)
	{
		_process_bone_hierarchy(&nodes[child], time, nodes, bones, final_transforms, transform);
	}
}

float AnimationSystem::_interpolate_geoset_alpha(float time, const GeosetAnim* anim)
{
	// If there is only 1 entry, this is a constant scale from time = 0 (it would not make sense for the time to be > 0)
	if (anim->alphas.size() == 1)
		return anim->alphas[0].key;

	FloatKey left, right;

	// Go through list of translation values over time and find the entries that this time fits between
	for (int i = 0; i < anim->alphas.size(); i++)
	{
		if (anim->alphas[i].time > time)
		{
			right = anim->alphas[i];
			break;
		}

		left = anim->alphas[i];
	}

	// Normalise the time value between the two timesteps found above for interpolation
	float normalised_time = (time - left.time) / (right.time - left.time);

	// Return LERP'd value
	if(anim->interp_mode == InterpolationMode::Linear)
		return glm::mix(left.key, right.key, normalised_time);
	
	// Don't interpolate, return the left key because we haven't reached the right time yet
	return left.key;
}

void AnimationSystem::_process_geoset_anims(float time, const std::vector<GeosetAnim>& geoset_anims, std::vector<float>& alpha_values)
{
	// Set default alphas to 1.0 (opaque), as not all animations might affect a geoset alpha value
	std::fill(alpha_values.begin(), alpha_values.end(), 1.0f);

	for (auto& anim : geoset_anims)
	{
		float alpha = _interpolate_geoset_alpha(time, &anim);

		alpha_values[anim.geoset_id] = alpha;
	}
}