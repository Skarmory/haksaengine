#pragma once

#include <glm/glm.hpp>

struct Ray
{
	glm::vec3 position;
	glm::vec3 direction;
};

bool intersect_triangle(const Ray& ray, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3& xsect_point);