#include "camera_controller_script.h"

#include "services.h"

#include "ecs/transform.h"

CameraControllerScript::CameraControllerScript(SystemOrdering order) : System(order)
{
	Services::get().get_event_manager()->subscribe("MoveCameraEvent", this);
}

void CameraControllerScript::update(float delta)
{
}

void CameraControllerScript::on_event(Event e)
{
	if (e.event_type == "MoveCameraEvent")
	{
		InputManager* input_man = Services::get().get_input_manager();

		const Entity& main_camera = Services::get().get_scene_manager()->get_main_camera();
		Transform* transform = main_camera.get_component<Transform>();

		 if (input_man->get_mouse().pressed(MouseButtonType::Button_2))
		{
			float xoff = input_man->get_mouse().x_offset();
			float yoff = input_man->get_mouse().y_offset();

			glm::vec3 cam_left = -glm::normalize(glm::cross(transform->get_forward(), WORLD_UP));
			glm::vec3 cam_forw = glm::normalize(glm::cross(cam_left, WORLD_UP));

			transform->translate_by(cam_left * xoff + cam_forw * yoff);
		}

		if (input_man->get_mouse().scroll() != 0.0f)
		{
			glm::vec3 scroll = transform->get_forward() * input_man->get_mouse().scroll();
			transform->translate_by(scroll);
		}
	}
}