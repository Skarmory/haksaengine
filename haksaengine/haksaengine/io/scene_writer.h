#pragma once

#include <string>
#include <fstream>

#include "globals.h"

#include "event/event_handler.h"

// Class that writes out all scene information to a .scene file.
// To be used typically by the mapeditor to save a user created scene
class SceneWriter : public EventHandler
{
public:

	HAKSAENGINE_API SceneWriter(void);
	HAKSAENGINE_API ~SceneWriter(void) = default;

	void on_event(Event e) override;

	// Write to given scene filename
	HAKSAENGINE_API void write(const std::string& filename);

private:

	// Write out the blueprints
	void _write_blueprints(std::ofstream& os);

	// Write out the entity instances
	void _write_entities(std::ofstream& os);

	// Write out the terrain data
	void _write_terrain(std::ofstream& os);

private:

	std::vector<unsigned int> _entities;
	std::vector<unsigned int> _blueprints;

	const std::string _default_scene_name = "default";
};