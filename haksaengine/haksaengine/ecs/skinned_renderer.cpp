#include "ecs/skinned_renderer.h"

#include <glm/glm.hpp>

#include <iostream>

#include "services.h"
#include "ecs/camera.h"
#include "ecs/transform.h"
#include "ecs/animator.h"
#include "ecs/skinned_renderable.h"
#include "ecs/player.h"

#include "io/mdl.h"
#include "gfx/shader.h"

#include "asset_manager.h"

SkinnedRenderer::SkinnedRenderer(SystemOrdering order) : RenderLogicSystem(order), _need_sort(false)
{
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, (GLint*)&_instances_max);

	_instances_max /= sizeof(PerInstanceData);

	_per_instance_data = new PerInstanceData[_instances_max];
}

SkinnedRenderer::~SkinnedRenderer(void)
{
	delete[] _per_instance_data;
}

void SkinnedRenderer::update(float delta)
{
	if (_entities.size() == 0)
		return;

	AssetManager* asset_man = Services::get<AssetManager>();
	
	// Do frustum culling for efficiency
	std::vector<unsigned int> culled_entities = Services::get<SceneManager>()->cull_by_main_camera(_entities);

	if (culled_entities.size() == 0)
		return;

	if (_need_sort)
	{
		std::sort(_entities.begin(), _entities.end(), _order_for_render);
		_need_sort = false;
	}

	// Get camera info
	const Entity& main_camera = Services::get<SceneManager>()->get_main_camera();
	Camera* camera = main_camera.get_component<Camera>();
	Transform* camera_transform = main_camera.get_component<Transform>();

	// Update camera UBO
	CameraData camera_data;
	camera_data.view = camera->view_matrix;
	camera_data.projection = camera->projection_matrix;
	camera_data.position = camera_transform->get_position();

	update_uniform<CameraData>(CAMERA_UNIFORM_BIND_POINT, &camera_data);

	// Cache the currently bound shader and mdl so we can easily see when an expensive context switch occurs
	Shader* current_shader = nullptr;
	MDLFile* current_mdl   = nullptr;
	unsigned int instances = 0;

	// Have to set the current shader and mdl, so do the first entity outside of the loop
	Entity* entity = Services::get<EntityManager>()->get_entity(culled_entities[0]);

	SkinnedRenderable* renderable = entity->get_component<SkinnedRenderable>();
	Animator* animator = entity->get_component<Animator>();
	Transform* transform = entity->get_component<Transform>();
	Player* player = entity->get_component<Player>();

	MDLFile& mdl = asset_man->get_asset<MDLFile>(renderable->model);
	Shader& shader = asset_man->get_asset<Shader>(renderable->shader);

	current_shader = &shader;
	current_mdl = &mdl;

	use_shader(*current_shader);

	// Loop through all the rest of the entities and draw them
	for (int i = 0; i < culled_entities.size(); i++)
	{
		entity = Services::get<EntityManager>()->get_entity(culled_entities[i]);

		renderable = entity->get_component<SkinnedRenderable>();
		animator = entity->get_component<Animator>();
		transform = entity->get_component<Transform>();
		player = entity->get_component<Player>();

		mdl = asset_man->get_asset<MDLFile>(renderable->model);
		shader = asset_man->get_asset<Shader>(renderable->shader);

		// Shader context switch
		if (shader.get_id() != current_shader->get_id())
		{
			if(renderable->instanced)
				_draw_instanced(mdl, renderable, instances);

			// Set new shader to current
			current_shader = &shader;
			use_shader(*current_shader);
			instances = 0;
		}

		// Mesh context switch 
		if (mdl.get_id() != current_mdl->get_id())
		{
			if (renderable->instanced)
				_draw_instanced(mdl, renderable, instances);

			// Set new mdl to current
			current_mdl = &mdl;
			instances = 0;
		}

		if (renderable->instanced)
		{
			// Accumulate more instance data
			PerInstanceData* instance_data = &_per_instance_data[instances];
			instance_data->model = transform->get_transform();
			instance_data->animation_offset = animator->animation_offset;
			instance_data->frame_offset = animator->frame_offset;
			instance_data->player_colour = player ? player->colour : PlayerColour::DEFAULT;
			instance_data->alpha = 1.0f;

			instances++;

			// Maximum instances have been filled up
			if (instances == _instances_max)
			{
				_draw_instanced(mdl, renderable, instances);

				instances = 0;
			}
		}
		else
		{
			PerDrawDataSkinned per_draw;
			per_draw.model = transform->get_transform();
			per_draw.player_colour = player ? player->colour : PlayerColour::DEFAULT;
			std::copy(renderable->final_bone_transforms.begin(), renderable->final_bone_transforms.end(), per_draw.bones);

			_draw(mdl, renderable, &per_draw);
		}
	}
	
	if(instances > 0)
		_draw_instanced(mdl, renderable, instances);

	submit_commands();
}

void SkinnedRenderer::on_event(Event ev)
{
	if (ev.event_type == "EntityCreatedEvent")
	{
		unsigned int entity_id = ev.arguments[0].as_uint;
		Entity* entity = Services::get<EntityManager>()->get_entity(entity_id);

		if (entity->has_component<SkinnedRenderable>())
		{
			_need_sort = true;
			_entities.push_back(entity_id);
		}
	}
	else if (ev.event_type == "EntityDestroyedEvent")
	{
		unsigned int entity_id = ev.arguments[0].as_uint;
		std::vector<unsigned int>::iterator it;
		if ((it = std::find(_entities.begin(), _entities.end(), entity_id)) != _entities.end())
		{
			std::iter_swap(it, _entities.end() - 1);
			_entities.pop_back();
		}
	}
}

void SkinnedRenderer::_draw_instanced(const MDLFile& mdl, const SkinnedRenderable* renderable, unsigned int instances)
{
	// Draw each submesh
	for (auto& data : mdl.get_data())
	{
		// If this mesh is totally transparent, don't bother rendering it
		if (renderable->geoset_alphas[data.mesh_id] == 0.0f)
		{
			continue;
		}
		// If this submesh has mesh-wide transparency, add it to all the per instance data
		else if (renderable->geoset_alphas[data.mesh_id] != 1.0f)
		{
			for (int instance = 0; instance < instances; instance++)
			{
				_per_instance_data[instance].alpha = renderable->geoset_alphas[data.mesh_id];
			}
		}

		// Get submesh and texture data
		const Mesh* submesh = mdl.get_mesh(data.mesh_id);
		const Texture* diffuse = mdl.get_texture(data.texture_id);
		const Texture* animation_data = mdl.get_animation_texture();

		// Bind resources
		bind_mesh(*submesh);
		bind_texture(*diffuse);
		bind_texture(*animation_data);

		// Set per draw data
		PerDrawDataInstancing per_draw;
		per_draw.diffuse = diffuse->get_handle();
		per_draw.animation_data = animation_data->get_handle();

		update_uniform<PerDrawDataInstancing>(PER_DRAW_UNIFORM_BIND_POINT, &per_draw);
		update_uniform<PerInstanceData>(PER_INSTANCE_UNIFORM_BIND_POINT, _per_instance_data, instances);

		draw_indexed_instanced(PrimitiveType::Triangles, submesh->index_count(), 0, instances);
	}
}

void SkinnedRenderer::_draw(const MDLFile& mdl, const SkinnedRenderable* renderable, PerDrawDataSkinned* per_draw)
{
	for (auto& data : mdl.get_data())
	{
		// If this mesh is totally transparent, don't bother rendering it
		if (renderable->geoset_alphas[data.mesh_id] == 0.0f)
		{
			continue;
		}
		
		const Mesh* submesh = mdl.get_mesh(data.mesh_id);
		const Texture* diffuse = mdl.get_texture(data.texture_id);

		per_draw->alpha = renderable->geoset_alphas[data.mesh_id];
		per_draw->diffuse = diffuse->get_handle();

		bind_mesh(*submesh);
		bind_texture(*diffuse);

		update_uniform<PerDrawDataSkinned>(PER_DRAW_UNIFORM_BIND_POINT, per_draw);
		draw_indexed(PrimitiveType::Triangles, submesh->index_count(), 0);
	}
}

bool SkinnedRenderer::_order_for_render(unsigned int& left, unsigned int& right)
{
	EntityManager* entityman = Services::get().get_entity_manager();

	SkinnedRenderable* left_renderable = entityman->get_entity(left)->get_component<SkinnedRenderable>();
	SkinnedRenderable* right_renderable = entityman->get_entity(right)->get_component<SkinnedRenderable>();

	if (left_renderable->shader < right_renderable->shader)
		return true;

	if (left_renderable->model < right_renderable->model)
		return true;

	return false;
}