#pragma once

#include <glm/glm.hpp>

class Frustum
{
public:

	// Extract frustum planes from view-projection matrix
	void extract_planes(glm::mat4 vp);

	// Check if a point is inside the frustum.
	// Returns true if it is inside, false if not
	bool is_in_bounds(glm::vec3 point);

private:

	// Each of the planes that defines this frustum
	float left[4];
	float right[4];
	float top[4];
	float bottom[4];
	float near[4];
	float far[4];
};