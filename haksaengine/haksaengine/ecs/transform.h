#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "globals.h"

#include "ecs/component.h"

const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 WROLD_FOWARD = glm::vec3(0.0f, 0.0f, 1.0f);

// Component that contains spatial data about an entity
struct Transform : public Component<Transform>
{
	Transform(void) : _position(glm::vec3()), _rotation(glm::vec3()), _scale(glm::vec3(1.0))
	{}

	// Load in transform data from NamedVariantPack
	void load(NamedVariantPack* data) override
	{
		_position.x = data->get("px").as_float;
		_position.y = data->get("py").as_float;
		_position.z = data->get("pz").as_float;

		_rotation.x = data->get("rx").as_float;
		_rotation.y = data->get("ry").as_float;
		_rotation.z = data->get("rz").as_float;

		_scale.x = data->get("sx").as_float;
		_scale.y = data->get("sy").as_float;
		_scale.z = data->get("sz").as_float;
	}

	// Sets absolute translation
	void translate(glm::vec3 translation)
	{
		_position = translation;
	}

	// OFfsets current translation by given value
	void translate_by(glm::vec3 translation)
	{
		_position += translation;
	}

	// Return current translation
	glm::vec3 get_position(void) const
	{
		return _position;
	}

	// Sets absolute rotation
	void rotate(glm::vec3 rotation)
	{
		_rotation = rotation;
	}

	// Offsets current rotation by given value
	void rotate_by(glm::vec3 rotation)
	{
		_rotation += rotation;
	}

	// Return current rotation as euler angles
	glm::vec3 get_rotation(void) const
	{
		return _rotation;
	}

	// Sets absolute scale
	void scale(glm::vec3 scale)
	{
		_scale = scale;
	}

	// Offset current scale by given value
	void scale_by(glm::vec3 scale)
	{
		_scale += scale;
	}

	// Returns current scale
	glm::vec3 get_scale(void) const
	{
		return _scale;
	}

	// Composes transform, rotation, and scale into a 4x4 transform matrix
	glm::mat4 get_transform(void) const
	{
		glm::mat4 transform(1.0f);

		glm::quat q_rot(glm::vec3(glm::radians(_rotation.x), glm::radians(_rotation.y), glm::radians(_rotation.z)));

		transform = glm::translate(glm::mat4(1.0f), _position) * glm::toMat4(q_rot) * glm::scale(glm::mat4(1.0f), _scale);

		return transform;
	}

	// Get local forward
	glm::vec3 get_forward(void) const
	{
		glm::vec3 forward;

		// Construct rotation matrix and extract the 3rd column of it to get the forward vector
		glm::quat q_rot(glm::vec3(glm::radians(_rotation.x), glm::radians(_rotation.y), glm::radians(_rotation.z)));

		glm::mat4 rotation_matrix = glm::toMat4(q_rot);

		forward = rotation_matrix[2];

		return forward;
	}

private:
	glm::vec3 _position;
	glm::vec3 _rotation;
	glm::vec3 _scale;
};