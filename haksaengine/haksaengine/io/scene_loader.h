#pragma once

#include <fstream>

#include "globals.h"

#include "ecs/entity.h"

class SceneLoader
{
public:

	HAKSAENGINE_API SceneLoader(void) = default;
	HAKSAENGINE_API ~SceneLoader(void) = default;

	HAKSAENGINE_API void load(const std::string& filename);

private:

	void _parse_blueprints(std::ifstream& fs);

	void _parse_entities(std::ifstream& fs);

	void _parse_transient_component(std::ifstream& fs, Entity* entity, const std::string& component_name);

};
