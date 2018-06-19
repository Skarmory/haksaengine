#pragma once

#include "globals.h"
#include "ecs/component.h"

struct Renderable : public Component<Renderable>
{
	void load(NamedVariantPack* data) override
	{
		this->model = data->get("model").as_uint;
		this->shader = data->get("shader").as_uint;
	}

	unsigned int model;
	unsigned int shader;
};