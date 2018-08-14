#include "scene/shapes.h"

// AABB-Triangle intersection code taken from http://fileadmin.cs.lth.se/cs/Personal/Tomas_Akenine-Moller/code/tribox3.txt
// This is the most efficient AABB-Triangle intersection algorithm

// X tests
#define AXISTEST_X01(a, b, fa, fb)			   \
	p0 = a * v0.y - b * v0.z;			       	   \
	p2 = a * v2.y - b * v2.z;			       	   \
	if (p0 < p2) { min = p0; max = p2; } \
	else { min = p2; max = p0; } \
	rad = fa * half_diag.y + fb * half_diag.z;   \
	if (min > rad || max < -rad) return false;

#define AXISTEST_X2(a, b, fa, fb)			   \
	p0 = a * v0.y - b * v0.z;			           \
	p1 = a * v1.y - b * v1.z;			       	   \
	if (p0 < p1) { min = p0; max = p1; } \
	else { min = p1; max = p0; } \
	rad = fa * half_diag.y + fb * half_diag.z;   \
	if (min > rad || max < -rad) return false;

// Y tests
#define AXISTEST_Y02(a, b, fa, fb)			   \
	p0 = -a * v0.x + b * v0.z;		      	   \
	p2 = -a * v2.x + b * v2.z;	       	       	   \
	if (p0 < p2) { min = p0; max = p2; } \
	else { min = p2; max = p0; } \
	rad = fa * half_diag.x + fb * half_diag.z;   \
	if (min > rad || max < -rad) return false;

#define AXISTEST_Y1(a, b, fa, fb)			   \
	p0 = -a * v0.x + b * v0.z;		      	   \
	p1 = -a * v1.x + b * v1.z;	     	       	   \
	if (p0 < p1) { min = p0; max = p1; } \
	else { min = p1; max = p0; } \
	rad = fa * half_diag.x + fb * half_diag.z;   \
	if (min > rad || max < -rad) return false;

// Z tests
#define AXISTEST_Z12(a, b, fa, fb)			   \
	p1 = a * v1.x - b * v1.y;			           \
	p2 = a * v2.x - b * v2.y;			       	   \
	if (p2 < p1) { min = p2; max = p1; } \
	else { min = p1; max = p2; } \
	rad = fa * half_diag.x + fb * half_diag.y;   \
	if (min>rad || max<-rad) return 0;

#define AXISTEST_Z0(a, b, fa, fb)			   \
	p0 = a * v0.x - b * v0.y;				   \
	p1 = a * v1.x - b * v1.y;			           \
	if (p0 < p1) { min = p0; max = p1; } \
	else { min = p1; max = p0; } \
	rad = fa * half_diag.x + fb * half_diag.y;   \
	if (min > rad || max < -rad) return 0;


#define FINDMINMAX(x0,x1,x2,min,max) \
	min = max = x0;   \
	if (x1 < min) min = x1; \
	if (x1 > max) max = x1; \
	if (x2 < min) min = x2; \
	if (x2 > max) max = x2;


Triangle::Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) : v1(v1), v2(v2), v3(v3), normal(glm::normalize(glm::cross(v2-v1, v3-v1)))
{
}

Triangle::Triangle(void) : Triangle(glm::vec3(0.0, 1.0f, 0.0f), glm::vec3(-0.5, 0.0f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f))
{
}

Plane::Plane(glm::vec3 normal, glm::vec3 position) : normal(normal), position(position)
{
}

Plane::Plane(void) : Plane(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f))
{
}

AABB::AABB(glm::vec3 min, glm::vec3 max) : min(min), max(max), centre((min + max) / 2.0f), half_diag((max - min) / 2.0f)
{
}

AABB::AABB(void) : AABB(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f))
{
}

bool AABB::intersect(const Triangle& tri) const
{
	float min, max, p0, p1, p2, rad, fex, fey, fez;
	glm::vec3 normal, e0, e1, e2, v0, v1, v2;

	v0 = tri.v1 - centre;
	v1 = tri.v2 - centre;
	v2 = tri.v3 - centre;

	e0 = v1 - v0;
	e1 = v2 - v1;
	e2 = v0 - v2;

	fex = fabsf(e0.x);
	fey = fabsf(e0.y);
	fez = fabsf(e0.z);
	AXISTEST_X01(e0.z, e0.y, fez, fey);
	AXISTEST_Y02(e0.z, e0.x, fez, fex);
	AXISTEST_Z12(e0.y, e0.x, fey, fex);

	fex = fabsf(e1.x);
	fey = fabsf(e1.y);
	fez = fabsf(e1.z);
	AXISTEST_X01(e1.z, e1.y, fez, fey);
	AXISTEST_Y02(e1.z, e1.x, fez, fex);
	AXISTEST_Z0(e1.y, e1.x, fey, fex);

	fex = fabsf(e2.x);
	fey = fabsf(e2.y);
	fez = fabsf(e2.z);
	AXISTEST_X2(e2.z, e2.y, fez, fey);
	AXISTEST_Y1(e2.z, e2.x, fez, fex);
	AXISTEST_Z12(e2.y, e2.x, fey, fex);

	FINDMINMAX(v0.x, v1.x, v2.x, min, max);
	if (min > half_diag.x || max < -half_diag.x)
		return false;

	FINDMINMAX(v0.y, v1.y, v2.y, min, max);
	if (min > half_diag.y || max < -half_diag.y)
		return false;

	FINDMINMAX(v0.z, v1.z, v2.z, min, max);
	if (min > half_diag.z || max < -half_diag.z)
		return false;

	normal = glm::cross(e0, e1);

	if (!intersect(Plane(normal, v0)))
		return false;

	return true;
}

bool AABB::intersect(const Plane& plane) const
{
	glm::vec3 minv, maxv;
	float v;

	for (int i = 0; i < 3; i++)
	{
		v = plane.position[i];

		if (plane.normal[i] > 0.0f)
		{
			minv[i] = -half_diag[i] - v;
			maxv[i] = half_diag[i] - v;
		}
		else
		{
			minv[i] = half_diag[i] - v;
			maxv[i] = -half_diag[i] - v;
		}
	}

	if (glm::dot(plane.normal, minv) > 0.0f)
		return false;

	if (glm::dot(plane.normal, maxv) >= 0.0f)
		return true;

	return false;
}

bool AABB::intersect(const AABB& aabb) const
{
	for (int i = 0; i < 3; i++)
	{
		if (min[i] > aabb.max[i] || aabb.min[i] > max[i])
			return false;
	}

	return true;
}

bool AABB::intersect(glm::vec3 point) const
{
	if (point.x > min.x && point.x < max.x && point.y > min.y && point.y < max.y && point.z > min.z && point.z < max.z)
		return true;

	return false;
}