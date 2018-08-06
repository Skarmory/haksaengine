#pragma once

#include "globals.h"
#include "service.h"
#include "event/event.h"
#include "event/event_handler.h"

#include "ecs/entity.h"
#include "scene/terrain.h"
#include "scene/ray.h"

/* This class will deal with the managing scenes and certain aspects of them, such as spatial partitioning and camera management */
class SceneManager : public Service, public EventHandler
{
public:
	SceneManager(void);
	~SceneManager(void);

	// No copying
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

	// Handle subscribed events
	virtual void on_event(Event ev) override;

	// Return the main (active) camera entity by const reference
	HAKSAENGINE_API const Entity& get_main_camera(void) const;

	// Do frustum cull of all transformable entities in the scene and store them
	void cull_entities(void);

	// Do frustum cull of given entities by comparison to culled entities vector
	HAKSAENGINE_API std::vector<unsigned int> cull_by_main_camera(const std::vector<unsigned int>& entities);

	// Do frustum cull of given entities by given camera
	HAKSAENGINE_API std::vector<unsigned int> cull_by_camera(const std::vector<unsigned int>& entities, Entity* camera);

	// Create the scene terrain geometry
	HAKSAENGINE_API void create_terrain(unsigned int width, unsigned int height, const char* tileset);

	// Intersect a ray
	HAKSAENGINE_API Entity* intersect_entity(const Ray& ray);

	// Intersect a ray
	HAKSAENGINE_API TerrainVertex* intersect_terrain(const Ray& ray);

	// Draw terrain
	void draw_terrain(void);

private:
	Entity* main_camera;
	Terrain* _terrain;

	std::vector<unsigned int> _transformable_entities;
	std::vector<unsigned int> _culled_entities;
};