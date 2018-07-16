#include "ecs/basic_render_system.h"

#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "services.h"

#include "ecs/renderable.h"
#include "ecs/camera.h"
#include "ecs/transform.h"
#include "ecs/player.h"

#include "io/mdl.h"

#include "gfx/mesh.h"
#include "gfx/shader.h"
#include "gfx/texture.h"

BasicRenderSystem::BasicRenderSystem(void)
{
}

void BasicRenderSystem::update(float delta)
{
	AssetManager* asset_man = Services::get<AssetManager>();

	const Entity& main_camera = Services::get<SceneManager>()->get_main_camera();
	Camera* camera = main_camera.get_component<Camera>();
	Transform* camera_transform = main_camera.get_component<Transform>();

	CameraData camera_data;
	camera_data.view = glm::lookAt(camera_transform->get_position(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	camera_data.projection = glm::perspective(camera->fov, 800.0f / 600.0f, camera->near_plane, camera->far_plane);

	update_uniform<CameraData>(CAMERA_UNIFORM_BIND_POINT, camera_data);

	for (auto entity_id : _entities)
	{
		Entity* entity = Services::get<EntityManager>()->get_entity(entity_id);

		Transform* transform = entity->get_component<Transform>();
		Renderable* renderable = entity->get_component<Renderable>();
		Player* player = entity->get_component<Player>();

		MDLFile& mdl = asset_man->get_asset<MDLFile>(renderable->model);
		Shader& shader = asset_man->get_asset<Shader>(renderable->shader);

		use_shader(shader);

		PerDrawData per_draw_data;
		per_draw_data.model = transform->get_transform();
		per_draw_data.alpha = 1.0f;
		per_draw_data.player_colour = player ? player->colour : PlayerColour::DEFAULT;

		for (auto data : mdl.get_data())
		{
			const Mesh* mesh = mdl.get_mesh(data.mesh_id);
			const Texture* texture = mdl.get_texture(data.texture_id);

			bind_texture(*texture);

			per_draw_data.diffuse = texture->get_handle();

			update_uniform<PerDrawData>(PER_DRAW_UNIFORM_BIND_POINT, per_draw_data);

			bind_mesh(*mesh);
			
			draw_indexed(PrimitiveType::Triangles, mesh->index_count(), 0);
		}
	}
}

void BasicRenderSystem::on_event(Event ev)
{
	if (ev.event_type == "EntityCreatedEvent")
	{
		unsigned int entity_id = ev.arguments[0].as_uint;
		Entity* entity = Services::get<EntityManager>()->get_entity(entity_id);

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
}