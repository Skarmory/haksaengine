#include "scene/ray.h"

bool intersect_triangle(const Ray& ray, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3& xsect_point)
{
	glm::vec3 e1 = p2 - p1;
	glm::vec3 e2 = p3 - p1;

	glm::vec3 q = glm::cross(ray.direction, e2);

	float a = glm::dot(e1, q);

	if (a > -0.00001f && a < 0.00001)
		return false;

	float f = 1.0f / a;

	glm::vec3 s = ray.position - p1;

	float u = f * glm::dot(s, q);

	if (u < 0.0f)
		return false;

	glm::vec3 r = glm::cross(s, e1);

	float v = f * glm::dot(ray.direction, r);

	if (v < 0.0f || u + v > 1.0f)
		return false;

	float t = f * glm::dot(e2, r);

	xsect_point = (1.0f - u - v) * p1 + u * p2 + v * p3;

	return true;
}