#include "ecs/skinned_renderer.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "services.h"
#include "ecs/camera.h"
#include "ecs/transform.h"
#include "ecs/skinned_renderable.h"
#include "ecs/player.h"

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

		// Get components we need from entity
		SkinnedRenderable* renderable = entity->get_component<SkinnedRenderable>();
		Transform* transform = entity->get_component<Transform>();
		Player* player = entity->get_component<Player>();

		// Get assets
		MDLFile& mdl = asset_man->get_asset<MDLFile>(renderable->model);
		Shader& shader = asset_man->get_asset<Shader>(renderable->shader);

		shader.use();

		glm::vec3 camera_position = camera_transform->get_position();
		glm::mat4 model, view, projection;

		model = transform->get_transform();
		view = glm::lookAt(camera_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		projection = glm::perspective(camera->fov, 800.0f / 600.0f, camera->near_plane, camera->far_plane);

		GLint model_loc = glGetUniformLocation(shader.get_program(), "model");
		GLint view_loc = glGetUniformLocation(shader.get_program(), "view");
		GLint proj_loc = glGetUniformLocation(shader.get_program(), "projection");
		GLint bone_loc = glGetUniformLocation(shader.get_program(), "bones");
		GLint alpha_loc = glGetUniformLocation(shader.get_program(), "alpha");
		GLint pc_loc = glGetUniformLocation(shader.get_program(), "team_colour");

		glUniformMatrix4fv(model_loc, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(bone_loc, renderable->final_bone_transforms.size(), GL_FALSE, (const GLfloat*) renderable->final_bone_transforms.data());

		for (auto data : mdl.get_data())
		{
			const Mesh* mesh = mdl.get_mesh(data.mesh_id);
			const Texture* texture = mdl.get_texture(data.texture_id);

			glUniform1f(alpha_loc, renderable->geoset_alphas[data.mesh_id]);

			unsigned int player_colour = player ? player->colour : PlayerColour::DEFAULT;

			glUniform1ui(pc_loc, player_colour);

			mesh->bind();
			texture->bind(0);

			glDrawElements(GL_TRIANGLES, mesh->index_count(), GL_UNSIGNED_INT, 0);

			mesh->unbind();
		}
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