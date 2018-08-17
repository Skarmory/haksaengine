#include "scene_writer.h"

#include <algorithm>

#include "services.h"

SceneWriter::SceneWriter(void)
{
	Services::get().get_event_manager()->subscribe("AssetBlueprintLoaded", this);
	Services::get().get_event_manager()->subscribe("EntityCreatedEvent", this);
	Services::get().get_event_manager()->subscribe("EntityDestroyedEvent", this);
}

void SceneWriter::on_event(Event e)
{
	if (e.event_type == "AssetBlueprintLoaded")
	{
		_blueprints.push_back(e.arguments[0].as_uint);
	}
	else if (e.event_type == "EntityCreatedEvent")
	{
		_entities.push_back(e.arguments[0].as_uint);
	}
	else if (e.event_type == "EntityDestroyedEvent")
	{
		std::sort(_entities.begin(), _entities.end());

		auto it = std::lower_bound(_entities.begin(), _entities.end(), e.arguments[0].as_uint);

		std::iter_swap(it, _entities.end() - 1);

		_entities.erase(_entities.end() - 1);
	}
}

void SceneWriter::write(const std::string& filename, bool should_complete_path)
{
	std::string full_path;

	if (should_complete_path)
	{
		full_path = Services::get().get_asset_manager()->get_asset_directory_path();

		full_path += "scenes/" + (!filename.empty() ? filename : _default_scene_name) + ".scene";
	}
	else
	{
		full_path = filename;
	}

	std::ofstream os(full_path);

	_write_blueprints(os);

	_write_entities(os);

	_write_terrain(os);
}

void SceneWriter::_write_blueprints(std::ofstream& os)
{
	AssetManager* assetman = Services::get().get_asset_manager();

	os << "Blueprints:" << std::endl;
	os << "[" << std::endl;

	for (auto blueprint_id : _blueprints)
	{
		Blueprint& blueprint = assetman->get_asset<Blueprint>(blueprint_id);

		os << "\t" << blueprint.get_id() << "\t" << blueprint.get_name() << std::endl;
	}

	os << "]" << std::endl << std::endl;
}

void SceneWriter::_write_entities(std::ofstream& os)
{
	AssetManager* assetman = Services::get().get_asset_manager();
	EntityManager * entityman = Services::get().get_entity_manager();

	os << "Entities:" << std::endl;
	os << "[" << std::endl;

	for (auto entity_id : _entities)
	{
		Entity* entity = entityman->get_entity(entity_id);

		Blueprint& blueprint = assetman->get_asset<Blueprint>(entity->get_blueprint_id());

		os << "\t+" << blueprint.get_id() << std::endl;
		
		entity->save_transient_components(os);
	}

	os << "]" << std::endl << std::endl;
}

void SceneWriter::_write_terrain(std::ofstream& os)
{
	Terrain* terrain = Services::get().get_scene_manager()->get_terrain();

	terrain->save(os);
}