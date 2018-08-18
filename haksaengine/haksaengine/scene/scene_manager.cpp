#include "scene/scene_manager.h"

#include "services.h"
#include "ecs/camera.h"
#include "ecs/transform.h"

#include "terrain_gen.h"
#include "gfx/uniform_data.h"
#include "gfx/render_command.h"

SceneManager::SceneManager(void)
{
	Services::get<EventManager>()->subscribe("EntityCreatedEvent", this);
	Services::get<EventManager>()->subscribe("EntityDestroyedEvent", this);
}

SceneManager::~SceneManager(void)
{
	if (_terrain)
		delete _terrain;
}

void SceneManager::on_event(Event ev)
{
	if (ev.event_type == "EntityCreatedEvent")
	{
		// Grab this if it's a camera entity
		auto entity = Services::get<EntityManager>()->get_entity(ev.arguments[0].as_uint);
		if (entity->has_component<Camera>())
		{
			main_camera = entity;
		}
		else if(entity->has_component<Transform>())
		{
			_transformable_entities.push_back(ev.arguments[0].as_uint);
		}
	}
	else if (ev.event_type == "EntityDestroyedEvent")
	{
		auto it = std::find(_transformable_entities.begin(), _transformable_entities.end(), ev.arguments[0].as_uint);
		
		if (it != _transformable_entities.end())
		{
			std::iter_swap(it, _transformable_entities.end() - 1);

			_transformable_entities.pop_back();
		}
	}
}

const Entity& SceneManager::get_main_camera(void) const
{
	return *main_camera;
}

void SceneManager::cull_entities(void)
{
	_culled_entities = cull_by_camera(_transformable_entities, main_camera);
	std::sort(_culled_entities.begin(), _culled_entities.end());
}

std::vector<unsigned int> SceneManager::cull_by_main_camera(const std::vector<unsigned int>& entities)
{
	std::vector<unsigned int> culled;
	culled.reserve(entities.size());

	for (auto eid : entities)
	{
		if (std::binary_search(_culled_entities.begin(), _culled_entities.end(), eid))
			culled.push_back(eid);
	}

	return culled;
}

std::vector<unsigned int> SceneManager::cull_by_camera(const std::vector<unsigned int>& entities, Entity* camera)
{
	Camera* camera_data = camera->get_component<Camera>();
	std::vector<unsigned int> ret;

	for (auto eid : entities)
	{
		Entity* entity = Services::get<EntityManager>()->get_entity(eid);

		Transform* transform = entity->get_component<Transform>();
		
		glm::vec3 pos = transform->get_position();

		if (camera_data->frustum.is_in_bounds(pos))
			ret.push_back(eid);
	}

	return ret;
}

void SceneManager::create_terrain(unsigned int width, unsigned int height, const char* tileset)
{
	if (_terrain)
		return;

	_terrain = TerrainGenerator::generate(width, height, tileset);
}

Entity* SceneManager::intersect_entity(const Ray& ray)
{
	return nullptr;
}

TerrainVertexData* SceneManager::intersect_terrain(const Ray& ray)
{
	return _terrain->intersect(ray);
}

void SceneManager::draw_terrain(void)
{
	if (!_terrain)
		return;

	Camera* camera = main_camera->get_component<Camera>();
	Transform* camera_transform = main_camera->get_component<Transform>();

	// Update camera UBO
	CameraData camera_data;
	camera_data.view = camera->view_matrix;
	camera_data.projection = camera->projection_matrix;
	camera_data.position = camera_transform->get_position();

	UpdateBufferObjectCommand* update_uniforms = new UpdateBufferObjectCommand;
	update_uniforms->add_uniform(new Buffer<CameraData>(CAMERA_UNIFORM_BIND_POINT, &camera_data));

	std::vector<const RenderCommand*> cmd;
	cmd.push_back(update_uniforms);

	Services::get().get_renderer()->submit_render_commands(cmd);

	_terrain->draw();
}

Terrain* SceneManager::get_terrain(void)
{
	return _terrain;
}

Ray SceneManager::screen_to_world_ray(float x, float y)
{
	Camera* camera = main_camera->get_component<Camera>();
	Transform* transform = main_camera->get_component<Transform>();

	glm::mat4 projection = camera->projection_matrix;
	glm::mat4 view = camera->view_matrix;

	// Convert from screen coordinates back into NDC
	float px = ((2.0f * x) / camera->width) - 1.0f;
	float py = 1.0f - (2.0f * y) / camera->height;

	// Convert back into clip space
	glm::vec4 ray_eye = glm::inverse(projection) * glm::vec4(px, py, -1.0f, 1.0f);
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0);

	// Convert from clip to world space and normalise
	glm::vec3 ray_world = glm::normalize(glm::inverse(view) * ray_eye);

	Ray ray;
	ray.position = transform->get_position();
	ray.direction = ray_world;

	return ray;
}

void SceneManager::load_scene(const std::string& filename)
{
	loader.load(filename);
}

void SceneManager::load_terrain(std::ifstream& fs)
{
	_terrain = TerrainGenerator::generate(fs);
}