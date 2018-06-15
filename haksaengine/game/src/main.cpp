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

	unsigned int camera_blueprint_id = Services::get().get_asset_manager()->load_asset<Blueprint>("camera.bpr");
	unsigned int test_blueprint_id = Services::get().get_asset_manager()->load_asset<Blueprint>("test.bpr");

	const Blueprint& camera_blueprint = static_cast<const Blueprint&>( Services::get().get_asset_manager()->get_asset(camera_blueprint_id) );
	const Blueprint& test_blueprint = static_cast<const Blueprint&>(Services::get().get_asset_manager()->get_asset(test_blueprint_id));

	Services::get().get_entity_manager()->create_entity(&camera_blueprint);
	Services::get().get_entity_manager()->create_entity(&test_blueprint);

	e.run();

	return 0;
}