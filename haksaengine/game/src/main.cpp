#include <GL/glew.h>

#include "engine.h"
#include "services.h"
#include "io/blueprint.h"

#include <string>
#include <iostream>
#include <memory>

#include "ecs/entity_manager.h"
#include "ecs/entity.h"
#include "ecs/component.h"
#include "ecs/transform.h"
#include "ecs/animator.h"
#include "ecs/player.h"


int main(int argc, char** argv)
{
	Engine e;
	e.initialise();

	Services::get().get_asset_manager()->set_asset_directory_path("..\\..\\assets\\");

	unsigned int camera_blueprint_id = Services::get().get_asset_manager()->load_asset<Blueprint>("camera.bpr");
	unsigned int test_blueprint_id = Services::get().get_asset_manager()->load_asset<Blueprint>("archer.bpr");

	const Blueprint& camera_blueprint = Services::get().get_asset_manager()->get_asset<Blueprint>(camera_blueprint_id);

	const Blueprint& test_blueprint = Services::get().get_asset_manager()->get_asset<Blueprint>(test_blueprint_id);

	Services::get().get_entity_manager()->create_entity(&camera_blueprint);

	unsigned int entity_id;
	Entity* entity;
	Transform* transform;
	Animator* anim;
	Player* player;

	// Create first archer
	entity_id = Services::get().get_entity_manager()->create_entity(&test_blueprint);

	entity = Services::get().get_entity_manager()->get_entity(entity_id);

	player = new Player;
	player->colour = PlayerColour::RED;
	entity->add_component(player);

	// Create second archer
	entity_id = Services::get().get_entity_manager()->create_entity(&test_blueprint);

	entity = Services::get().get_entity_manager()->get_entity(entity_id);

	transform = entity->get_component<Transform>();
	transform->translate(glm::vec3(50.0f, 0.0f, 0.0f));

	anim = entity->get_component<Animator>();
	anim->current_animation = "Attack";

	player = new Player;
	player->colour = PlayerColour::BLUE;
	entity->add_component(player);
	
	// Create third archer
	entity_id = Services::get().get_entity_manager()->create_entity(&test_blueprint);

	entity = Services::get().get_entity_manager()->get_entity(entity_id);

	transform = entity->get_component<Transform>();
	transform->translate(glm::vec3(100.0f, 0.0f, 0.0f));

	anim = entity->get_component<Animator>();
	anim->current_animation = "Death";

	player = new Player;
	player->colour = PlayerColour::GREEN;
	entity->add_component(player);	

	e.run();

	return 0;
}