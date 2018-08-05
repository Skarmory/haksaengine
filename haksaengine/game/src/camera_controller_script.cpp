#include "camera_controller_script.h"

#include <iostream>

#include "services.h"

#include "ecs/transform.h"

CameraControllerScript::CameraControllerScript(SystemOrdering order) : System(order)
{
}

void CameraControllerScript::update(float delta)
{
	const Entity& main_camera = Services::get().get_scene_manager()->get_main_camera();
	Transform* transform = main_camera.get_component<Transform>();

	InputManager* input_man = Services::get().get_input_manager();

	if (input_man->get_mouse().pressed(MouseButtonType::Button_2))
	{
		float xoff = input_man->get_mouse().x_offset();
		float yoff = input_man->get_mouse().y_offset();

		std::cout << xoff << ", " << yoff << std::endl;

		glm::vec3 cam_left = -glm::normalize(glm::cross(transform->get_forward(), WORLD_UP)) * 0.1f;
		glm::vec3 cam_forw = glm::normalize(glm::cross(cam_left, WORLD_UP)) * 0.1f;

		transform->translate_by(cam_left * xoff + cam_forw * yoff);
	}

	if (input_man->get_mouse().scroll() != 0.0f)
	{
		glm::vec3 scroll = transform->get_forward() * input_man->get_mouse().scroll();
		transform->translate_by(scroll);
	}
}

void CameraControllerScript::on_event(Event e)
{

}