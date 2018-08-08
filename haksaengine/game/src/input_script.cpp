#include "input_script.h"

#include "services.h"

#include "scene/ray.h"

#include "ecs/camera.h"
#include "ecs/transform.h"

InputScript::InputScript(SystemOrdering order) : System(order)
{
}

void InputScript::update(float delta)
{
	const Mouse& mouse = Services::get().get_input_manager()->get_mouse();
	if (mouse.pressed(MouseButtonType::Button_1))
	{
		const Mouse& mouse = Services::get().get_input_manager()->get_mouse();

		Ray ray = Services::get().get_scene_manager()->screen_to_world_ray(mouse.x(), mouse.y());

		Terrain* terrain = Services::get().get_scene_manager()->get_terrain();

		TerrainVertexData* vertex;
		
		if (vertex = terrain->intersect(ray))
		{
			vertex->texture = 2;

			terrain->update(vertex);
		}
	}
}

void InputScript::on_event(Event e)
{
}