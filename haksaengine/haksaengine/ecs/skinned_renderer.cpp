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

SkinnedRenderer::SkinnedRenderer(void) : _camera(CAMERA_UNIFORM_BIND_POINT), _per_draw(PER_DRAW_UNIFORM_BIND_POINT)
{
	_camera.initialise();
	_per_draw.initialise();
}

void SkinnedRenderer::update(float delta)
{
	AssetManager* asset_man = Services::get().get_asset_manager();

	// Get camera info
	const Entity& main_camera = Services::get().get_scene_manager()->get_main_camera();
	Camera* camera = main_camera.get_component<Camera>();
	Transform* camera_transform = main_camera.get_component<Transform>();

	// Update camera UBO
	CameraUniform camera_data;
	camera_data.view = glm::lookAt(camera_transform->get_position(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	camera_data.projection = glm::perspective(camera->fov, 800.0f / 600.0f, camera->near_plane, camera->far_plane);
	_camera.update(camera_data);
	_camera.bind();

	// Draw each renderable entity
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

		// Setup per draw uniform buffer
		PerDrawUniform per_draw_data;
		per_draw_data.model = transform->get_transform();
		std::copy(renderable->final_bone_transforms.begin(), renderable->final_bone_transforms.end(), per_draw_data.bones);

		for (auto data : mdl.get_data())
		{
			const Mesh* mesh = mdl.get_mesh(data.mesh_id);
			const Texture* texture = mdl.get_texture(data.texture_id);
			BindlessTextureHandle texture_handle = texture->get_handle();

			glMakeTextureHandleResidentARB(texture_handle);

			per_draw_data.alpha = renderable->geoset_alphas[data.mesh_id];
			per_draw_data.player_colour = player ? player->colour : PlayerColour::DEFAULT;
			per_draw_data.diffuse = texture_handle;

			_per_draw.update(per_draw_data);
			_per_draw.bind();

			mesh->bind();

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