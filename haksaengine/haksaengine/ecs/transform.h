#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "globals.h"

#include "ecs/component.h"

struct Transform : public Component<Transform>
{
	Transform(void) : position(glm::vec3()), rotation(glm::vec3())
	{}

	void translate(glm::vec3 translation)
	{
		position = translation;
	}

	void translate_by(glm::vec3 translation)
	{
		position += translation;
	}

	glm::vec3 get_position(void) const
	{
		return position;
	}

	void rotate(glm::vec3 rotation)
	{
		this->rotation = rotation;
	}

	void rotate_by(glm::vec3 rotation)
	{
		this->rotation += rotation;
	}

	glm::vec3 get_rotation(void) const
	{
		return rotation;
	}

	glm::mat4x4 get_transform(void) const
	{
		glm::mat4 transform(1.0f);

		glm::quat q_rot(glm::vec3(glm::radians(rotation.x), glm::radians(rotation.y), glm::radians(rotation.z)));

		transform = glm::translate(transform, position);
		transform = glm::toMat4(q_rot) * transform;


		return transform;
	}

private:
	glm::vec3 position;
	glm::vec3 rotation;
};