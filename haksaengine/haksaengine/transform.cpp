////#define GLM_ENABLE_EXPERIMENTAL
//
//#include "transform.h"
//
//#include <glm/vec3.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//
////#include <glm/gtc/quaternion.hpp>
////#include <glm/gtx/quaternion.hpp>
//
//void Transform::translate(glm::vec3 translation)
//{
//	position = translation;
//}
//
//void Transform::translate_by(glm::vec3 translation)
//{
//	position += translation;
//}
//
//void Transform::rotate(glm::vec3 rotation)
//{
//	this->rotation = rotation;
//}
//
//void Transform::rotate_by(glm::vec3 rotation)
//{
//	this->rotation += rotation;
//}
//
//glm::vec3 Transform::get_rotation(void) const
//{
//	return rotation;
//}
//
//glm::mat4x4 Transform::get_transform(void) const
//{
//	glm::mat4x4 transform;
//
//	//glm::quat x = glm::angleAxis(rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
//	//glm::quat y = glm::angleAxis(rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
//	//glm::quat z = glm::angleAxis(rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
//	//
//	//glm::quat q_rot = x * y * z;
//	//
//	//transform = transform * glm::toMat4(q_rot);
//	transform = glm::translate(transform, position);
//
//	return transform;
//}