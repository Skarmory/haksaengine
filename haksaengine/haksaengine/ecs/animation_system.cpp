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

	//std::cout << culled_entities.size() << std::endl;

	if (culled_entities.size() > 100)
	{
		//std::cout << culled_entities.size() << std::endl;
		int interval = _entities.size() / 4;

		std::vector<std::thread> threads;
		threads.reserve(4);

		for (int thread_count = 0; thread_count < 4; thread_count++)
		{
			std::thread t(&AnimationSystem::_do_animator_update, this, thread_count * interval, interval, delta);
			threads.push_back(std::move(t));
		}

		for (auto& thread : threads)
			thread.join();

		//
		interval = culled_entities.size() / 4;

		threads.clear();
		for (int thread_count = 0; thread_count < 4; thread_count++)
		{
			std::thread t(&AnimationSystem::_do_animation_update, this, thread_count * interval, interval, culled_entities, delta);
			threads.push_back(std::move(t));
		}

		for (auto& thread : threads)
			thread.join();
	}
	else
	{
		_do_animator_update(0, _entities.size(), delta);
		_do_animation_update(0, culled_entities.size(), culled_entities, delta);
	}

	
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
			_entities.erase(it);
	}
}

void AnimationSystem::_do_animator_update(int start, int count, float delta)
{
	//for (auto entity_id : _entities)
	for(int i = start; i < (start + count); i++)
	{
		Entity* entity = Services::get<EntityManager>()->get_entity(_entities[i]);

		Animator* animator = entity->get_component<Animator>();
		SkinnedRenderable* renderable = entity->get_component<SkinnedRenderable>();

		animator->current_time += delta;

		MDLFile& data = Services::get<AssetManager>()->get_asset<MDLFile>(renderable->model);
		const Animation* animation = data.get_animation(animator->current_animation);

		if (animator->current_time > animation->duration)
		{
			animator->current_time = std::fmod(animator->current_time, animation->duration);
		}
	}
}

void AnimationSystem::_do_animation_update(int start, int count, const std::vector<unsigned int>& culled_entities, float delta)
{
	//for (auto entity_id : culled_entities)
	for(int i = start; i < (start + count); i++)
	{
		Entity* entity = Services::get<EntityManager>()->get_entity(culled_entities[i]);

		Animator* animator = entity->get_component<Animator>();
		SkinnedRenderable* renderable = entity->get_component<SkinnedRenderable>();

		animator->accumulated_frames++;

		if (animator->accumulated_frames >= animator->lod_intervals[animator->lod])
			animator->accumulated_frames = 0;
		else
			continue;

		MDLFile& data = Services::get<AssetManager>()->get_asset<MDLFile>(renderable->model);
		const Animation* animation = data.get_animation(animator->current_animation);

		_process_bone_hierarchy(animation->root_pose_node, animator->current_time, animation->pose_nodes, data.get_bones(), renderable->final_bone_transforms, glm::mat4(1.0f));
		_process_geoset_anims(animator->current_time, animation->geoset_anims, renderable->geoset_alphas);
	}
}

void AnimationSystem::_process_bone_hierarchy(const BonePoseNode* node, float time, const std::vector<BonePoseNode>& nodes, const std::vector<Bone>& bones, std::vector<glm::mat4>& final_transforms, glm::mat4 parent_transform)
{
	// Get the correct animation values via interpolation
	glm::vec3 scale = _interpolate_scale(time, node);
	glm::quat rotation = _interpolate_rotation(time, node);
	glm::vec3 position = _interpolate_position(time, node);

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

glm::vec3 AnimationSystem::_interpolate_scale(float time, const BonePoseNode* pose)
{
	// If there is only 1 entry, this is a constant scale from time = 0 (it would not make sense for the time to be > 0)
	if (pose->scales.size() == 1)
		return pose->scales[0].key;

	Vector3Key left, right;

	// Go through list of scale values over time and find the entries that this time fits between
	for (int i = 0; i < pose->scales.size(); i++)
	{
		if (pose->scales[i].time > time)
		{
			right = pose->scales[i];
			break;
		}

		left = pose->scales[i];
	}

	// Normalise the time value between the two timesteps found above for interpolation
	float normalised_time = (time - left.time) / (right.time - left.time);

	// Return LERP'd value
	return glm::mix(left.key, right.key, normalised_time);
}

glm::quat AnimationSystem::_interpolate_rotation(float time, const BonePoseNode* pose)
{
	// If there is only 1 entry, this is a constant scale from time = 0 (it would not make sense for the time to be > 0)
	if (pose->rotations.size() == 1)
	{
		return pose->rotations[0].key;
	}

	QuaternionKey left, right;

	// Go through list of rotation values over time and find the entries that this time fits between
	for (int i = 0; i < pose->rotations.size(); i++)
	{
		if (pose->rotations[i].time >= time)
		{
			right = pose->rotations[i];
			break;
		}

		left = pose->rotations[i];
	}

	// Normalise the time value between the two timesteps found above for interpolation
	float normalised_time = (time - left.time) / (right.time - left.time);	

	// Return SLERP'd value
	return glm::slerp(left.key, right.key, normalised_time);
}

glm::vec3 AnimationSystem::_interpolate_position(float time, const BonePoseNode* pose)
{
	// If there is only 1 entry, this is a constant scale from time = 0 (it would not make sense for the time to be > 0)
	if (pose->positions.size() == 1)
		return pose->positions[0].key;

	Vector3Key left, right;

	// Go through list of translation values over time and find the entries that this time fits between
	for (int i = 0; i < pose->positions.size(); i++)
	{
		if (pose->positions[i].time > time)
		{
			right = pose->positions[i];
			break;
		}

		left = pose->positions[i];
	}

	// Normalise the time value between the two timesteps found above for interpolation
	float normalised_time = (time - left.time) / (right.time - left.time);

	// Return LERP'd value
	return glm::mix(left.key, right.key, normalised_time);
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