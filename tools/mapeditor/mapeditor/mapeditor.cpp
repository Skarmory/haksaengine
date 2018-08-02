#include "mapeditor.h"

#include "ecs/transform.h"
#include "ecs/animator.h"

#include "camera_controller_script.h"

mapeditor::mapeditor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	_main_loop_timer = new QTimer(this);
	connect(_main_loop_timer, SIGNAL(timeout()), this, SLOT(main_loop()));

	_engine = new Engine(EngineMode::Editor);

	_main_loop_timer->start(0);
}

mapeditor::~mapeditor(void)
{
	delete _main_loop_timer;
	delete _engine;
}

void mapeditor::main_loop(void)
{
	if (_engine->get_state() == EngineState::Uninitialised)
	{
		_engine->initialise();

		ui.openGLWidget->set_engine(_engine);

		Services::get<AssetManager>()->set_asset_directory_path("../../../assets/");

		_initialise();

		_setup_default_scene();
	}

	if (_engine->get_state() == EngineState::Ready)
	{	
		ui.openGLWidget->update();
	}
}

void mapeditor::_setup_default_scene(void)
{
	const Blueprint& bpr = Services::get<AssetManager>()->load_and_get_asset<Blueprint>("camera.bpr");
	const Blueprint& archer_bpr = Services::get<AssetManager>()->load_and_get_asset<Blueprint>("archer.bpr");

	Services::get().get_scene_manager()->create_terrain(32, 32);

	Services::get<EntityManager>()->create_and_get_entity(&bpr);

	Entity& archer = Services::get<EntityManager>()->create_and_get_entity(&archer_bpr);

	//Transform* transform = archer.get_component<Transform>();
	//transform->translate(glm::vec3(i * 50.0f, 0.0f, j * 50.0f));

	Animator* anim = archer.get_component<Animator>();
	anim->current_animation = "Run";
}

void mapeditor::_initialise(void)
{
	SystemManager* sysman = Services::get().get_system_manager();

	sysman->create<CameraControllerScript>(SystemOrdering(0, UpdatePriority::POSTINPUT, 0));
}