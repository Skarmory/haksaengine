#pragma once

#include <vector>
#include <array>

#include <glm/glm.hpp>

#include "scene/terrain.h"
#include "scene/shapes.h"
#include "ecs/entity.h"
#include "ecs/collider.h"


struct TerrainVertexData;


struct TerrainTriangle : public Triangle
{
	TerrainTriangle(TerrainVertexData* v1, TerrainVertexData* v2, TerrainVertexData* v3);

	TerrainVertexData *tv1, *tv2, *tv3;
};

class TerrainQuadTree
{
public:
	TerrainQuadTree(AABB aabb);
	~TerrainQuadTree(void);

	void add(TerrainTriangle* triangle);


	std::unordered_set<TerrainTriangle*> get_intersections(Entity* entity);

private:
	
	// Nodes that define areas of the world space and contain terrain vertices
	class TerrainQuadTreeNode
	{
	public:
		TerrainQuadTreeNode(AABB aabb);
		~TerrainQuadTreeNode(void);

		void add(TerrainTriangle* triangle);

		void gather_intersections(std::unordered_set<TerrainTriangle*>& intersects, const AABB& aabb);

	private:
		static const int _objects_per_node = 30;

		unsigned int _count;

		AABB _aabb;

		TerrainQuadTreeNode *_tl, *_tr, *_bl, *_br;

		std::vector<TerrainTriangle*> _objects;
	};

	TerrainQuadTreeNode* _origin;
};