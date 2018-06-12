#pragma once

#include "gfx/mesh.h"
#include "gfx/shader.h"

class MeshRenderer
{
public:
	MeshRenderer(Mesh* mesh, Shader* program);

	void render(void);

private:
	Mesh* _mesh;
	Shader* _program;
};
