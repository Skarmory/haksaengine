#pragma once

#include "globals.h"

#include "event/event.h"
#include "event/event_handler.h"

#include "ecs/entity.h"

/* This class will deal with the managing scenes and certain aspects of them, such as spatial partitioning and camera management */
class SceneManager : public EventHandler
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

private:
	Entity* main_camera;
};