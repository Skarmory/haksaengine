#include "ecs/renderer.h"

#include <algorithm>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "services.h"

#include "ecs/renderable.h"
#include "ecs/camera.h"
#include "ecs/transform.h"

#include "gfx/mesh.h"
#include "gfx/shader.h"

Renderer::Renderer(void)
{
	Services::get().get_event_manager()->subscribe("AssetMeshLoaded", this);
}

void Renderer::update(float delta)
{
	// DEBUG - This code needs to go elsewhere in future to accomodate multiple renderer variants
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// DEBUG

	AssetManager* asset_man = Services::get().get_asset_manager();

	const Entity& main_camera = Services::get().get_scene_manager()->get_main_camera();
	Camera* camera = main_camera.get_component<Camera>();
	Transform* camera_transform = main_camera.get_component<Transform>();

	for (auto entity_id : _entities)
	{
		Entity* entity = Services::get().get_entity_manager()->get_entity(entity_id);

		Renderable* renderable = entity->get_component<Renderable>();

		const Mesh& mesh = static_cast<const Mesh&>(asset_man->get_asset(renderable->mesh));
		const Shader& shader = static_cast<const Shader&>(asset_man->get_asset(renderable->shader));

		mesh.bind();

		shader.use();

		// DEBUG CODE - Just hardcoded for testing
		glm::vec3 camera_position = camera_transform->get_position();
		glm::mat4 model, view, projection;

		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::lookAt(camera_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		projection = glm::perspective(camera->fov, 800.0f / 600.0f, camera->near_plane, camera->far_plane);

		GLint model_loc = glGetUniformLocation(shader.get_program(), "model");
		GLint view_loc = glGetUniformLocation(shader.get_program(), "view");
		GLint proj_loc = glGetUniformLocation(shader.get_program(), "projection");

		glUniformMatrix4fv(model_loc, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &projection[0][0]);

		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
		// DEBUG CODE

		mesh.unbind();
	}
}

void Renderer::on_event(Event ev)
{
	if (ev.event_type == "EntityCreatedEvent")
	{
		unsigned int entity_id = ev.arguments[0].as_uint;
		Entity* entity = Services::get().get_entity_manager()->get_entity(entity_id);

		if (entity->has_component<Renderable>())
			_entities.push_back(entity_id);
	}
	else if (ev.event_type == "EntityDestroyedEvent")
	{
		unsigned int entity_id = ev.arguments[0].as_uint;
		std::vector<unsigned int>::iterator it;
		if ((it = std::find(_entities.begin(), _entities.end(), entity_id)) != _entities.end())
			_entities.erase(it);
	}
	else if (ev.event_type == "AssetMeshLoaded")
	{
		// Should store?
	}
}