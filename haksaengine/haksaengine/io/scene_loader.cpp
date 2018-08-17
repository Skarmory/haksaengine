#include "io/scene_loader.h"

#include "services.h"

void SceneLoader::load(const std::string& filename)
{
	std::string full_path = Services::get().get_asset_manager()->get_asset_directory_path();

	full_path += "scenes/" + filename;

	std::ifstream fs(full_path);

	std::string line;
	std::string data;

	while (std::getline(fs, line))
	{
		if (line.empty())
			continue;

		auto idx = line.find(':');

		data = line.substr(0, idx);

		if (data == "Blueprints")
		{
			_parse_blueprints(fs);
		}
		else if (data == "Entities")
		{
			_parse_entities(fs);
		}
		else if (data == "Terrain")
		{
			Services::get().get_scene_manager()->load_terrain(fs);
		}
	}
}

void SceneLoader::_parse_blueprints(std::ifstream& fs)
{
	AssetManager* assetman = Services::get().get_asset_manager();

	std::string line;
	std::string data;

	unsigned int blueprint_id;

	while (std::getline(fs, line))
	{
		if (line.find('[') != std::string::npos)
			continue;

		if (line.find(']') != std::string::npos)
			break;

		auto idx1 = line.find_first_not_of('\t');
		auto idx2 = line.find_first_of('\t', idx1);

		data = line.substr(idx1, idx2 - idx1);

		blueprint_id = std::stoul(data);

		idx1 = line.find_first_not_of('\t', idx2);
		
		data = line.substr(idx1, line.size() - idx1);

		unsigned int loaded_id = assetman->load_asset<Blueprint>(data.c_str());

		assert(blueprint_id == loaded_id);
	}
}

void SceneLoader::_parse_entities(std::ifstream& fs)
{
	AssetManager* assetman = Services::get().get_asset_manager();
	EntityManager* entityman = Services::get().get_entity_manager();

	std::string line;
	std::string data;

	unsigned int blueprint_id;
	Entity* entity = nullptr;

	while (std::getline(fs, line))
	{
		if (line.find('[') != std::string::npos)
			continue;

		if (line.find(']') != std::string::npos)
			break;

		auto idx1 = line.find_first_not_of('\t');
		auto idx2 = line.find_first_of('\t', idx1);

		data = line.substr(idx1, idx2 - idx1);	

		if (data[0] == '+')
		{
			data.erase(data.begin());
			blueprint_id = std::stoul(data);

			entity = &entityman->create_and_get_entity(&assetman->get_asset<Blueprint>(blueprint_id));
		}
		else
		{
			data.erase(data.end() - 1);
			_parse_transient_component(fs, entity, data);
		}
	}
}

void SceneLoader::_parse_transient_component(std::ifstream& fs, Entity* entity, const std::string& component_name)
{
	std::string line;
	std::string type, name, value;

	BaseComponent* component = Services::get().get_component_manager()->create_component(component_name.c_str());

	NamedVariantPack datapack;


	while (std::getline(fs, line))
	{
		if (line.find('[') != std::string::npos)
			continue;

		if (line.find(']') != std::string::npos)
			break;

		auto idx1 = line.find_first_not_of('\t');
		auto idx2 = line.find_first_of(' ', idx1);

		type = line.substr(idx1, idx2 - idx1);
		
		idx1 = line.find_first_not_of(' ', idx2);
		idx2 = line.find_first_of(' ', idx1);

		name = line.substr(idx1, idx2 - idx1);

		idx1 = line.find_first_not_of(' ', idx2);
		idx2 = line.find_first_of(';', idx1);

		value = line.substr(idx1, idx2 - idx1);

		Variant v;

		if (type == "float")
		{
			v.type = Variant::Type::FLOAT;
			v.as_float = std::stof(value);
		}
		else if (type == "int")
		{
			v.type = Variant::Type::INTEGER;
			v.as_int = std::stoi(value);
		}
		else if (type == "uint")
		{
			v.type = Variant::Type::UNSIGNEDINT;
			v.as_uint = std::stoul(value);
		}
		else if (type == "bool")
		{
			v.type = Variant::Type::BOOL;
			v.as_bool = value == "true" ? true : false;
		}
		else if (type == "string")
		{
			v.type = Variant::Type::POINTER;
			std::string* ptr = new std::string(value);
			v.as_ptr = ptr;
		}

		datapack.set(name.c_str(), v);
	}

	component->load(&datapack);

	entity->add_component(component);
}