#include <GL/glew.h>

#include "engine.h"
#include "gfx/shader.h"
#include "gfx/shader_program.h"
#include "gfx/mesh_loader.h"

#include "transform.h"
#include "ecs/renderer.h"

#include <string>
#include <iostream>
#include <memory>

const std::string asset_path = "../../assets/";

int main(int argc, char** argv)
{
	Transform t;
	Renderer r;

	auto x = t.get_transform();

	Engine e;
	e.initialise();

	e.run();
}