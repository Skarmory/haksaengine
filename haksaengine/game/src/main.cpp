#include <GL/glew.h>

#include "engine.h"
#include "gfx/shader.h"
#include "gfx/shader_program.h"
#include "gfx/mesh_loader.h"

#include "transform.h"


#include <string>
#include <iostream>
#include <memory>

const std::string asset_path = "../../assets/";

int main(int argc, char** argv)
{
	MeshLoader m;
	Transform t;

	auto x = t.get_transform();

	Engine e;
	e.initialise();

	Mesh* mesh = m.load_mesh(asset_path + "outverts.txt");

	Shader vs(asset_path + "shaders/basic_vshader.vs", GL_VERTEX_SHADER);
	Shader fs(asset_path + "shaders/basic_fshader.fs", GL_FRAGMENT_SHADER);

	ShaderProgram program;
	program.attach_shader(&vs);
	program.attach_shader(&fs);
	std::cout << program.link() << std::endl;

	program.use();

	e.run();
}