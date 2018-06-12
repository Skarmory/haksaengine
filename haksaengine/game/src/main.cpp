#include <GL/glew.h>

#include "engine.h"

#include "io/loader.h"
#include "gfx/mesh.h"
#include "gfx/shader.h"

#include <string>
#include <iostream>
#include <memory>


int main(int argc, char** argv)
{
	Engine e;
	e.initialise();

	Services::get().get_asset_manager()->set_asset_directory_path("..\\..\\assets\\");
	Services::get().get_asset_manager()->load_asset<Blueprint>("test");

	e.run();

	return 0;
}