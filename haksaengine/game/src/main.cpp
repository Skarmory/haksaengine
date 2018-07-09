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

class TestComponent : public Component<TestComponent>
{
	void load(NamedVariantPack* data) override
	{

	}
};

int main(int argc, char** argv)
{
	Engine e;
	e.initialise();

	TestComponent c;

	Services::get().get_asset_manager()->set_asset_directory_path("..\\..\\assets\\");

	unsigned int camera_blueprint_id = Services::get().get_asset_manager()->load_asset<Blueprint>("camera.bpr");
	unsigned int test_blueprint_id = Services::get().get_asset_manager()->load_asset<Blueprint>("test2.bpr");

	const Blueprint& camera_blueprint = Services::get().get_asset_manager()->get_asset<Blueprint>(camera_blueprint_id);

	const Blueprint& test_blueprint = Services::get().get_asset_manager()->get_asset<Blueprint>(test_blueprint_id);

	Services::get().get_entity_manager()->create_entity(&camera_blueprint);
	unsigned int entity_id = Services::get().get_entity_manager()->create_entity(&test_blueprint);

	entity_id = Services::get().get_entity_manager()->create_entity(&test_blueprint);

	Entity* entity = Services::get().get_entity_manager()->get_entity(entity_id);
	Transform* transform = entity->get_component<Transform>();
	Animator* anim = entity->get_component<Animator>();
	anim->current_animation = "Attack";
	transform->translate(glm::vec3(50.0f, 0.0f, 0.0f));

	entity_id = Services::get().get_entity_manager()->create_entity(&test_blueprint);

	entity = Services::get().get_entity_manager()->get_entity(entity_id);
	transform = entity->get_component<Transform>();
	anim = entity->get_component<Animator>();
	anim->current_animation = "Death";
	transform->translate(glm::vec3(100.0f, 0.0f, 0.0f));

	e.run();

	return 0;
}