#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <array>

#include "globals.h"

struct Triangle
{
	HAKSAENGINE_API Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
	HAKSAENGINE_API Triangle(void);

	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 v3;
	glm::vec3 normal;
};

struct Plane
{
	HAKSAENGINE_API Plane(glm::vec3 normal, glm::vec3 position);
	HAKSAENGINE_API Plane(void);

	glm::vec3 normal;
	glm::vec3 position;
};

// Axis aligned bounding box
struct AABB
{
	HAKSAENGINE_API AABB(glm::vec3 min, glm::vec3 max);
	HAKSAENGINE_API AABB(void);

	HAKSAENGINE_API bool intersect(Triangle tri);
	HAKSAENGINE_API bool intersect(Plane plane);
	HAKSAENGINE_API bool intersect(AABB aabb);

	glm::vec3 min;
	glm::vec3 max;

	glm::vec3 centre;
	glm::vec3 half_diag;
};