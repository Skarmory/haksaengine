#include <GL/glew.h>

#include "engine.h"
#include "services.h"
#include "io/blueprint.h"

#include <string>
#include <iostream>
#include <memory>
#include <random>

#include "ecs/entity_manager.h"
#include "ecs/entity.h"
#include "ecs/component.h"
#include "ecs/transform.h"
#include "ecs/animator.h"
#include "ecs/player.h"

#include "ecs/system_ordering.h"
#include "scene/terrain.h"
#include "scene/terrain_gen.h"

#include "camera_controller_script.h"
#include "input_script.h"

#include "scene/shapes.h"

int main(int argc, char** argv)
{
	Triangle triangle(
		glm::vec3(100.0f, 0.0f, 0.0f),
		glm::vec3(200.0f, 0.0f, 0.0f),
		glm::vec3(100.0f, 1.0f, 0.0f)
	);

	AABB aabb(
		glm::vec3(-10.0f, -10.0f, -10.0f),
		glm::vec3(10.0f, 10.0f, 10.0f)
	);

	bool xsect = aabb.intersect(triangle);

	std::mt19937 generator(999);
	std::uniform_real_distribution<float> dis(0.0f, 1.0f);

	Engine e(EngineMode::Game);
	e.initialise();

	Services::get().get_asset_manager()->set_asset_directory_path("../../../assets/");

	unsigned int camera_blueprint_id = Services::get().get_asset_manager()->load_asset<Blueprint>("camera.bpr");
	unsigned int test_blueprint_id = Services::get().get_asset_manager()->load_asset<Blueprint>("archer.bpr");
	unsigned int crate_bpr_id = Services::get<AssetManager>()->load_asset<Blueprint>("crate.bpr");

	const Blueprint& camera_blueprint = Services::get().get_asset_manager()->get_asset<Blueprint>(camera_blueprint_id);

	const Blueprint& test_blueprint = Services::get().get_asset_manager()->get_asset<Blueprint>(test_blueprint_id);

	const Blueprint& crate_bpr = Services::get<AssetManager>()->get_asset<Blueprint>(crate_bpr_id);

	Services::get().get_entity_manager()->create_entity(&camera_blueprint);

	unsigned int entity_id;
	Entity* entity;
	Transform* transform;
	Animator* anim;
	Player* player;

	for (int i = -1; i < 1; i++)
	for( int j = -1; j < 1; j++)
	{
		entity_id = Services::get().get_entity_manager()->create_entity(&test_blueprint);

		entity = Services::get().get_entity_manager()->get_entity(entity_id);

		transform = entity->get_component<Transform>();
		transform->translate(glm::vec3(i * 50.0f, 0.0f, j * 50.0f));

		anim = entity->get_component<Animator>();
		anim->current_animation = "Attack";
		anim->current_time = dis(generator);

		player = new Player;
		player->colour = (PlayerColour)std::abs(j % 3);
		entity->add_component(player);
	}

	Services::get().get_scene_manager()->create_terrain(6u, 6u, "basic.tileset");

	Services::get().get_system_manager()->create<CameraControllerScript>(SystemOrdering(0, UpdatePriority::POSTINPUT, 0));
	Services::get().get_system_manager()->create<InputScript>(SystemOrdering(0, UpdatePriority::POSTINPUT, 1));

	auto xsects = Services::get().get_scene_manager()->get_terrain()->intersect(entity);

	glm::vec3 average_normal(0.0f, 0.0f, 0.0f);
	glm::vec3 average_pos(0.0f, 0.0f, 0.0f);
	for (auto tri : xsects)
	{
		average_normal += tri->normal;
		average_pos += tri->v1;
		average_pos += tri->v2;
		average_pos += tri->v3;
	}

	average_normal = glm::normalize(average_normal / (float)xsects.size());
	average_pos = average_pos / (float)xsects.size();

	Collider* collider = entity->get_component<Collider>();
	AABB tmp;

	do 
	{
		glm::mat4 locscale = glm::mat4(1.0f);
		locscale = glm::scale(locscale, transform->get_scale());
		locscale = glm::translate(locscale, transform->get_position());

		glm::vec3 min = locscale * glm::vec4(collider->aabb.min, 1.0f);
		glm::vec3 max = locscale * glm::vec4(collider->aabb.max, 1.0f);

		tmp = AABB(min, max);

		if (tmp.intersect(average_pos))
		{
			transform->translate_by(average_normal);
		}
	} 
	while (tmp.intersect(average_pos));

	e.run();

	return 0;
}