#include "frustum.h"

void Frustum::extract_planes(glm::mat4 vp)
{
	// Sets planes as plane equation coefficients
	// Equation: ax + by + cz + d = 0

	// Index -> Coefficient:
	//		0 -> a, 1 -> b, 2 -> c, 3->d

	float magnitude;

	// Set left clipping plane
	left[0] = vp[0][3] + vp[0][0];
	left[1] = vp[1][3] + vp[1][0];
	left[2] = vp[2][3] + vp[2][0];
	left[3] = vp[3][3] + vp[3][0];

	magnitude = std::sqrtf(left[0] * left[0] + left[1] * left[1] + left[2] * left[2]);
	left[0] /= magnitude;
	left[1] /= magnitude;
	left[2] /= magnitude;
	left[3] /= magnitude;

	// Set right clipping plane
	right[0] = vp[0][3] - vp[0][0];
	right[1] = vp[1][3] - vp[1][0];
	right[2] = vp[2][3] - vp[2][0];
	right[3] = vp[3][3] - vp[3][0];

	magnitude = std::sqrtf(right[0] * right[0] + right[1] * right[1] + right[2] * right[2]);
	right[0] /= magnitude;
	right[1] /= magnitude;
	right[2] /= magnitude;
	right[3] /= magnitude;

	// Set bottom clipping plane
	bottom[0] = vp[0][3] + vp[0][1];
	bottom[1] = vp[1][3] + vp[1][1];
	bottom[2] = vp[2][3] + vp[2][1];
	bottom[3] = vp[3][3] + vp[3][1];

	magnitude = std::sqrtf(bottom[0] * bottom[0] + bottom[1] * bottom[1] + bottom[2] * bottom[2]);
	bottom[0] /= magnitude;
	bottom[1] /= magnitude;
	bottom[2] /= magnitude;
	bottom[3] /= magnitude;

	// Set top clipping plane
	top[0] = vp[0][3] - vp[0][1];
	top[1] = vp[1][3] - vp[1][1];
	top[2] = vp[2][3] - vp[2][1];
	top[3] = vp[3][3] - vp[3][1];

	magnitude = std::sqrtf(top[0] * top[0] + top[1] * top[1] + top[2] * top[2]);
	top[0] /= magnitude;
	top[1] /= magnitude;
	top[2] /= magnitude;
	top[3] /= magnitude;

	// Set near clipping plane
	near[0] = vp[0][3] + vp[0][2];
	near[1] = vp[1][3] + vp[1][2];
	near[2] = vp[2][3] + vp[2][2];
	near[3] = vp[3][3] + vp[3][2];

	magnitude = std::sqrtf(near[0] * near[0] + near[1] * near[1] + near[2] * near[2]);
	near[0] /= magnitude;
	near[1] /= magnitude;
	near[2] /= magnitude;
	near[3] /= magnitude;

	// Set far clipping plane
	far[0] = vp[0][3] - vp[0][2];
	far[1] = vp[1][3] - vp[1][2];
	far[2] = vp[2][3] - vp[2][2];
	far[3] = vp[3][3] - vp[3][2];

	magnitude = std::sqrtf(far[0] * far[0] + far[1] * far[1] + far[2] * far[2]);
	far[0] /= magnitude;
	far[1] /= magnitude;
	far[2] /= magnitude;
	far[3] /= magnitude;
}

bool Frustum::is_in_bounds(glm::vec3 point)
{
	// Test a series of inequalities to check whether the the point is on the inside of the frustum

	if ((point.x * left[0] + point.y * left[1] + point.z * left[2] + left[3]) < 0)
		return false;

	if ((point.x * right[0] + point.y * right[1] + point.z * right[2] + right[3]) < 0)
		return false;

	if ((point.x * top[0] + point.y * top[1] + point.z * top[2] + top[3]) < 0)
		return false;

	if ((point.x * bottom[0] + point.y * bottom[1] + point.z * bottom[2] + bottom[3]) < 0)
		return false;

	if ((point.x * near[0] + point.y * near[1] + point.z * near[2] + near[3]) < 0)
		return false;

	if ((point.x * far[0] + point.y * far[1] + point.z * far[2] + far[3]) < 0)
		return false;

	return true;
}