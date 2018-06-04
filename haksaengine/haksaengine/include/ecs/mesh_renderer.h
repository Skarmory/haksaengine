#pragma once

#include "globals.h"
#include "ecs/component.h"
#include "gfx/mesh.h"
#include "gfx/shader_program.h"

struct HAKSAENGINE_API MeshRenderer : public Component
{
	Mesh* mesh;
	ShaderProgram* shader;
};