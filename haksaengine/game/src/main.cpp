#include <GL/glew.h>

#include "engine.h"
#include "io/blueprint.h"

#include <string>
#include <iostream>
#include <memory>


int main(int argc, char** argv)
{
	Engine e;
	e.initialise();

	Services::get().get_asset_manager()->set_asset_directory_path("..\\..\\assets\\");
	unsigned int blueprint_id = Services::get().get_asset_manager()->load_asset<Blueprint>("test");
	const Blueprint& blueprint = static_cast<const Blueprint&>( Services::get().get_asset_manager()->get_asset(blueprint_id) );

	Services::get().get_entity_manager()->create_entity(&blueprint);

	e.run();

	return 0;
}