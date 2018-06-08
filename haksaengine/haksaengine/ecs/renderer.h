#pragma once

#include "globals.h"
#include "ecs/component.h"
#include "gfx/mesh_renderer.h"

struct Renderer : public Component
{
	MeshRenderer* mesh_renderer;
};