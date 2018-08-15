#pragma once

#include <glm/glm.hpp>
#include <set>

#include "ecs/system.h"
#include "ecs/transform.h"
#include "scene/shapes.h"
#include "scene/quadtree.h"

struct TerrainTriangle;

class CollisionResponse : public System
{
public:
	CollisionResponse(SystemOrdering order);

	void update(float delta) override;
	void on_event(Event e) override;

private:

	const glm::vec3 _gravity = glm::vec3(0.0f, -9.8f, 0.0f);
};