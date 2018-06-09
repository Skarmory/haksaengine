#include "blueprint_loader.h"

BlueprintLoader::BlueprintLoader(const std::string& blueprint_directory) : Loader<Blueprint>(blueprint_directory.c_str())
{
}

Blueprint* BlueprintLoader::load(const char* name)
{
	return new Blueprint;
}