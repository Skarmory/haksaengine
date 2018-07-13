#include "blueprint_loader.h"

#include <fstream>
#include <exception>

#include "services.h"

#include "io//mdl.h"
#include "gfx/shader.h"
#include "gfx/texture.h"

BlueprintLoader::BlueprintLoader(const std::string& blueprint_directory) : Loader(blueprint_directory, ".bpr")
{
}

Blueprint* BlueprintLoader::load(const std::string& name)
{
	Blueprint* blueprint = new Blueprint;

	std::string path = get_path(name);

	std::ifstream fs(path);

	std::string line, value;

	while (std::getline(fs, line))
	{
		auto idx1 = line.find_first_not_of('\t', 0);
		auto idx2 = line.find_first_of(':', idx1);

		value = line.substr(idx1, idx2 - idx1);

		if (value == "Components")
		{
			parse_components(fs, blueprint);
		}
	}

	return blueprint;
}

void BlueprintLoader::parse_components(std::ifstream& fs, Blueprint* bp)
{
	std::string line, value;

	while (std::getline(fs, line))
	{
		if (line.find_first_not_of(' ') == std::string::npos)
			break;

		auto idx1 = line.find_first_not_of('\t', 0);
		auto idx2 = line.find_first_of(':', idx1);

		value = line.substr(idx1, idx2 - idx1);

		if (Services::get<ComponentManager>()->is_registered(value.c_str()))
		{
			BaseComponent* comp = Services::get<ComponentManager>()->create_component(value.c_str());
			parse_component_data(fs, comp);
			bp->add_component(comp);
		}
		else
		{
			throw std::runtime_error("Component instantiator for type " + value + " not found.\nHas it been registered with the AssetManager?");
		}
	}
}

void BlueprintLoader::parse_component_data(std::ifstream& fs, BaseComponent* component)
{
	std::string line, name, value, type;

	size_t idx1, idx2;

	NamedVariantPack datapack;

	while (std::getline(fs, line))
	{
		if (line[line.size() - 1] == ']')
			break;
		else if (line[line.size() - 1] == '[')
			continue;

		idx1 = line.find_first_not_of('\t', 0);
		idx2 = line.find_first_of(' ', idx1);

		type = line.substr(idx1, idx2 - idx1);

		idx1 = line.find_first_not_of(' ', idx2);
		idx2 = line.find_first_of(' ', idx1);

		name = line.substr(idx1, idx2 - idx1);

		idx1 = line.find_first_not_of(' ', idx2);
		idx2 = line.find_first_of(';', idx1);

		value = line.substr(idx1, idx2 - idx1);

		Variant data;

		if (type == "float")
		{
			data.type = Variant::Type::FLOAT;
			data.as_float = std::stof(value);
		}
		else if (type == "int")
		{
			data.type = Variant::Type::INTEGER;
			data.as_int = std::stoi(value);
		}
		else if (type == "uint")
		{
			data.type = Variant::Type::UNSIGNEDINT;
			data.as_uint = std::stoul(value);
		}
		else if (type == "bool")
		{
			data.type = Variant::Type::BOOL;
			if (value == "true")
				data.as_bool = true;
			else if (value == "false")
				data.as_bool = false;
		}
		else if (type == "string")
		{
			data.type = Variant::Type::POINTER;
			std::string* ptr = new std::string(value);
			data.as_ptr = ptr;
		}
		else if (type == "external")
		{
			data.type = Variant::Type::UNSIGNEDINT;

			// These types specify another file that might need loading.
			// Rather than give the type directly, they give an id to the resource to the blueprint
			if (name == "model")
			{
				data.as_uint = Services::get<AssetManager>()->load_asset<MDLFile>(value.c_str());
			}
			else if (name == "shader")
			{
				data.as_uint = Services::get<AssetManager>()->load_asset<Shader>(value.c_str());
			}
		}

		datapack.set(name.c_str(), data);
	}

	component->load(&datapack);
}