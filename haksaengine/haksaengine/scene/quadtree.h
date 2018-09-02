#pragma once

#include <vector>
#include <set>

#include <glm/glm.hpp>

#include "scene/terrain.h"
#include "scene/shapes.h"
#include "scene/terrain_vertex_data.h"
#include "ecs/entity.h"
#include "ecs/collider.h"

struct TerrainVertexData;

struct TerrainTriangle : public Triangle
{
	TerrainTriangle(TerrainVertexData* v1, TerrainVertexData* v2, TerrainVertexData* v3);

	void _sync(void)
	{
		this->v1 = tv1->position;
		this->v2 = tv2->position;
		this->v3 = tv3->position;
	}

	TerrainVertexData *tv1, *tv2, *tv3;
};

class TerrainQuadTree
{
public:
	TerrainQuadTree(AABB aabb);
	~TerrainQuadTree(void);

	void add(TerrainTriangle* triangle);

	std::set<TerrainTriangle*> get_intersections(Entity* entity);
	std::set<TerrainTriangle*> get_intersections(const Ray& ray);

private:
	
	// Nodes that define areas of the world space and contain terrain vertices
	class TerrainQuadTreeNode
	{
	public:
		TerrainQuadTreeNode(AABB aabb);
		~TerrainQuadTreeNode(void);

		void add(TerrainTriangle* triangle);

		void gather_intersections(std::set<TerrainTriangle*>& intersects, const AABB& aabb);
		void gather_intersections(std::set<TerrainTriangle*>& intersects, const Ray& ray);

	private:
		static const int _objects_per_node = 30;

		unsigned int _count;

		AABB _aabb;

		TerrainQuadTreeNode *_tl, *_tr, *_bl, *_br;

		std::vector<TerrainTriangle*> _objects;
	};

	std::vector<TerrainTriangle*> _all_objects;
	TerrainQuadTreeNode* _origin;
};