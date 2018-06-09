#pragma once

#include <unordered_map>
#include <functional>
#include <typeindex>

#include "globals.h"
#include "io/loader.h"
#include "ecs/component.h"

class Blueprint
{

};

/* Class that handles loading Blueprints from disk */
class BlueprintLoader : public Loader<Blueprint>
{
public:
	BlueprintLoader(const std::string& blueprint_directory_path);

	// Construct a Blueprint
	Blueprint* load(const char* name) override;

	// Registers a component by name
	template<class ComponentType>
	void register_component(const std::string& component_id)
	{
		std::function<ComponentType*(void)> constructor = [](void) { return new ComponentType; };
		_component_type_map[component_id] = constructor;
	}

private:
	std::unordered_map<std::string, std::function<BaseComponent*(void)>> _component_type_map;
};