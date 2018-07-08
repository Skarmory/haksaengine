#include "ecs/skinned_renderer.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "services.h"
#include "ecs/camera.h"
#include "ecs/transform.h"
#include "ecs/skinned_renderable.h"

#include "io/mdl.h"
#include "gfx/shader.h"

void SkinnedRenderer::update(float delta)
{
	AssetManager* asset_man = Services::get().get_asset_manager();

	const Entity& main_camera = Services::get().get_scene_manager()->get_main_camera();
	Camera* camera = main_camera.get_component<Camera>();
	Transform* camera_transform = main_camera.get_component<Transform>();

	for (auto entity_id : _entities)
	{
		Entity* entity = Services::get().get_entity_manager()->get_entity(entity_id);

		SkinnedRenderable* renderable = entity->get_component<SkinnedRenderable>();

		MDLFile& mdl = static_cast<MDLFile&>(asset_man->get_asset(renderable->model));
		Shader& shader = static_cast<Shader&>(asset_man->get_asset(renderable->shader));

		shader.use();

		// DEBUG CODE - Just hardcoded for testing
		glm::vec3 camera_position = camera_transform->get_position();
		glm::mat4 model, view, projection;

		model = glm::mat4(1.0f);
		view = glm::lookAt(camera_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		projection = glm::perspective(camera->fov, 800.0f / 600.0f, camera->near_plane, camera->far_plane);

		GLint model_loc = glGetUniformLocation(shader.get_program(), "model");
		GLint view_loc = glGetUniformLocation(shader.get_program(), "view");
		GLint proj_loc = glGetUniformLocation(shader.get_program(), "projection");
		GLint bone_loc = glGetUniformLocation(shader.get_program(), "bones");

		/*for (int i = 1; i < renderable->frame_final_bone_transforms.size(); i++)
		{
			renderable->frame_final_bone_transforms[i] = glm::mat4(0.0f);
		}*/

		glUniformMatrix4fv(model_loc, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(bone_loc, renderable->frame_final_bone_transforms.size(), GL_FALSE, (const GLfloat*) renderable->frame_final_bone_transforms.data());

		for (auto data : mdl.get_data())
		{
			const Mesh* mesh = mdl.get_mesh(data.mesh_id);
			const Texture* texture = mdl.get_texture(data.texture_id);

			mesh->bind();
			texture->bind(0);

			glDrawElements(GL_TRIANGLES, mesh->index_count(), GL_UNSIGNED_INT, 0);

			mesh->unbind();
		}
		// DEBUG CODE
	}
}

void SkinnedRenderer::on_event(Event ev)
{
	if (ev.event_type == "EntityCreatedEvent")
	{
		unsigned int entity_id = ev.arguments[0].as_uint;
		Entity* entity = Services::get().get_entity_manager()->get_entity(entity_id);

		if (entity->has_component<SkinnedRenderable>())
			_entities.push_back(entity_id);
	}
	else if (ev.event_type == "EntityDestroyedEvent")
	{
		unsigned int entity_id = ev.arguments[0].as_uint;
		std::vector<unsigned int>::iterator it;
		if ((it = std::find(_entities.begin(), _entities.end(), entity_id)) != _entities.end())
			_entities.erase(it);
	}
}