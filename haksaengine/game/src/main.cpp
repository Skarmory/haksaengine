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
	std::mt19937 generator(999);
	std::uniform_real_distribution<float> dis(0.0f, 1.0f);

	Engine e(EngineMode::Game);
	e.initialise();

	Services::get().get_asset_manager()->set_asset_directory_path("../../../assets/");

	unsigned int camera_blueprint_id = Services::get().get_asset_manager()->load_asset<Blueprint>("camera.bpr");
	unsigned int test_blueprint_id = Services::get().get_asset_manager()->load_asset<Blueprint>("units/archer.bpr");
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

	int entities = 0;
	for (int i = -20; i < 20; i++)
	for (int j = -20; j < 20; j++)
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

		entities++;
	}

	Services::get().get_scene_manager()->create_terrain(32u, 32u, "basic.tileset");

	Services::get().get_system_manager()->create<CameraControllerScript>(SystemOrdering(0, UpdatePriority::POSTINPUT, 0));
	Services::get().get_system_manager()->create<InputScript>(SystemOrdering(0, UpdatePriority::POSTINPUT, 1));

	while (e.get_state() != EngineState::Quit)
	{
		e.one_frame();
	}

 	return 0;
}