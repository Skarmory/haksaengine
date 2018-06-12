#pragma once

#include <unordered_map>
#include <fstream>

#include "globals.h"
#include "io/loader.h"
#include "ecs/component.h"
#include "io/blueprint.h"

class BaseComponent;

/* Class that handles loading Blueprints from disk */
class BlueprintLoader : public Loader
{
public:
	BlueprintLoader(const std::string& blueprint_directory_path);

	// Construct a Blueprint
	Blueprint* load(const std::string& name) override;

private:
	void parse_components(std::ifstream& fs, Blueprint* bp);
	void parse_component_data(std::ifstream& fs, BaseComponent* component);
};