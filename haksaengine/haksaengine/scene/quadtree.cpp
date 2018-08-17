#include "quadtree.h"

#include "ecs/transform.h"

TerrainTriangle::TerrainTriangle(TerrainVertexData* v1, TerrainVertexData* v2, TerrainVertexData* v3)
	: tv1(v1), tv2(v2), tv3(v3), Triangle(v1->position, v2->position, v3->position)
{
}

TerrainQuadTree::TerrainQuadTree(AABB aabb)
{
	_origin = new TerrainQuadTreeNode(aabb);
}

TerrainQuadTree::~TerrainQuadTree(void)
{
	delete _origin;
}

void TerrainQuadTree::add(TerrainTriangle* triangle)
{
	_origin->add(triangle);
}

TerrainQuadTree::TerrainQuadTreeNode::TerrainQuadTreeNode(AABB aabb) 
	: _aabb(aabb), _tl(nullptr), _tr(nullptr), _bl(nullptr), _br(nullptr)
{
}

TerrainQuadTree::TerrainQuadTreeNode::~TerrainQuadTreeNode(void)
{
	for (auto object : _objects)
		delete object;

	if (_tl)
	{
		delete _tl;
		delete _tr;
		delete _bl;
		delete _br;
	}
}

std::set<TerrainTriangle*> TerrainQuadTree::get_intersections(Entity* entity)
{
	std::set<TerrainTriangle*> ret;

	Transform* transform = entity->get_component<Transform>();
	Collider* collider = entity->get_component<Collider>();
	
	glm::mat4 locscale = transform->get_transform_scale_translate();

	glm::vec3 min = locscale * glm::vec4(collider->aabb.min, 1.0f);
	glm::vec3 max = locscale * glm::vec4(collider->aabb.max, 1.0f);

	AABB tmp(min, max);

	_origin->gather_intersections(ret, tmp);

	return ret;
}

void TerrainQuadTree::TerrainQuadTreeNode::add(TerrainTriangle* triangle)
{
	// If has subnodes
	if (_tl)
	{
		if (_tl->_aabb.intersect(*triangle))
			_tl->add(triangle);

		if (_tr->_aabb.intersect(*triangle))
			_tr->add(triangle);

		if (_bl->_aabb.intersect(*triangle))
			_bl->add(triangle);

		if (_br->_aabb.intersect(*triangle))
			_br->add(triangle);
	}
	else
	{
		if (_objects.size() < _objects_per_node)
		{
			_objects.push_back(triangle);
		}
		else
		{
			AABB tl(
				_aabb.centre - _aabb.half_diag,
				_aabb.centre + glm::vec3(0.0f, _aabb.half_diag.y, 0.0f)
			);

			AABB tr(
				_aabb.centre - glm::vec3(0.0f, _aabb.half_diag.y, _aabb.half_diag.z),
				_aabb.centre + glm::vec3(_aabb.half_diag.x, _aabb.half_diag.y, 0.0f)
			);

			AABB bl(
				_aabb.centre - glm::vec3(_aabb.half_diag.x, _aabb.half_diag.y, 0.0f),
				_aabb.centre + glm::vec3(0.0f, _aabb.half_diag.y, _aabb.half_diag.z)
			);

			AABB br(
				_aabb.centre - glm::vec3(0.0f, _aabb.half_diag.y, 0.0f),
				_aabb.centre + _aabb.half_diag
			);

			_tl = new TerrainQuadTreeNode(tl);
			_tr = new TerrainQuadTreeNode(tr);
			_bl = new TerrainQuadTreeNode(bl);
			_br = new TerrainQuadTreeNode(br);

			_objects.push_back(triangle);

			for (auto& object : _objects)
			{
				if (_tl->_aabb.intersect(*object))
					_tl->add(object);

				if (_bl->_aabb.intersect(*object))
					_bl->add(object);

				if (_tr->_aabb.intersect(*object))
					_tr->add(object);

				if (_br->_aabb.intersect(*object))
					_br->add(object);
			}

			// This quadtree has subdivided, this vector isn't necessary anymore
			_objects.clear();
			_objects.shrink_to_fit();
		}
	}
}

void TerrainQuadTree::TerrainQuadTreeNode::gather_intersections(std::set<TerrainTriangle*>& intersects, const AABB& aabb)
{
	if (_tl)
	{
		if (aabb.intersect(_tl->_aabb))
			_tl->gather_intersections(intersects, aabb);

		if (aabb.intersect(_tr->_aabb))
			_tr->gather_intersections(intersects, aabb);

		if (aabb.intersect(_bl->_aabb))
			_bl->gather_intersections(intersects, aabb);

		if (aabb.intersect(_br->_aabb))
			_br->gather_intersections(intersects, aabb);
	}
	else
	{
		for (auto& object : _objects)
		{
			if (aabb.intersect(*object))
			{
				intersects.insert(object);
				/*intersects.push_back(&object);*/
			}
		}
	}
}