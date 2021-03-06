#include "mapeditor.h"

#include <filesystem>

#include "terrainpalettewidget.h"

#include "ecs/transform.h"
#include "ecs/animator.h"

#include "input_signal_processor.h"
#include "camera_controller_script.h"
#include "unit_placer.h"

#include "editor_state.h"

mapeditor::mapeditor(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	palette.show();

	save_action = new  QAction("Save scene", this);
	auto menu = menuBar()->addMenu("File");
	menu->addAction(save_action);

	connect(save_action, SIGNAL(triggered()), this, SLOT(save_scene()));

	save_dialog.setDefaultSuffix("scene");

	_main_loop_timer = new QTimer(this);
	connect(_main_loop_timer, SIGNAL(timeout()), this, SLOT(main_loop()));

	_init_timer = new QTimer(this);
	connect(_init_timer, SIGNAL(timeout()), this, SLOT(initialise()));

	_init_timer->start(0);

	_engine = new Engine(EngineMode::Editor);
}

mapeditor::~mapeditor(void)
{
	delete _writer;
	delete _main_loop_timer;
	delete _engine;

	EditorState::swap_state(nullptr);

	delete save_action;
}

void mapeditor::closeEvent(QCloseEvent* event)
{
	palette.close();

	QMainWindow::closeEvent(event);
}

void mapeditor::initialise(void)
{
	_init_timer->stop();

	delete _init_timer;

	_engine->initialise();

	ui.openGLWidget->set_engine(_engine);

	Services::get<AssetManager>()->set_asset_directory_path("../../../assets/");

	_writer = new SceneWriter;

	_load_assets();

	_initialise();

	palette.init_palettes();

	_setup_default_scene();

	working = false;

	_main_loop_timer->start(0);
}

void mapeditor::main_loop(void)
{
	bool desired = false;

	if (working.compare_exchange_strong(desired, true))
	{	
		ui.openGLWidget->update();

		working = false;
	}
}

void mapeditor::save_scene(void)
{
	QString filter = "Scene (*.scene)";
	auto filename = save_dialog.getSaveFileName(this, tr("Save scene"), "", filter, &filter);

	_writer->write(filename.toStdString(), false);
}

void mapeditor::_load_assets(void)
{
	AssetManager* assetman = Services::get().get_asset_manager();

	std::string unit_blueprint_dir = std::string(assetman->get_asset_directory_path()) + "blueprints/units";
	std::string object_blueprint_dir = std::string(assetman->get_asset_directory_path()) + "blueprints/objects";

	std::string unit_dir = "units/";
	std::string object_dir = "objects/";

	std::vector<const Blueprint*> unit_ids;
	std::vector<const Blueprint*> object_ids;

	for (auto& path : std::experimental::filesystem::directory_iterator(unit_blueprint_dir))
	{
		std::string filename = path.path().filename().string();
		const Blueprint* blueprint = &assetman->load_and_get_asset<Blueprint>((unit_dir + filename).c_str());

		unit_ids.push_back(blueprint);
	}

	static_cast<UnitPaletteWidget*>(palette.get_palette(PaletteType::Unit))->set_units(unit_ids);

	for (auto& path : std::experimental::filesystem::directory_iterator(object_blueprint_dir))
	{
		std::string filename = path.path().filename().string();
		const Blueprint* blueprint = &assetman->load_and_get_asset<Blueprint>((object_dir + filename).c_str());

		object_ids.push_back(blueprint);
	}

	static_cast<UnitPaletteWidget*>(palette.get_palette(PaletteType::Unit))->set_objects(object_ids);

}

void mapeditor::_setup_default_scene(void)
{
	const Blueprint& bpr = Services::get<AssetManager>()->load_and_get_asset<Blueprint>("camera.bpr");

	Services::get().get_scene_manager()->create_terrain(32u, 32u, "basic.tileset");

	Services::get<EntityManager>()->create_and_get_entity(&bpr);

	palette.update_palettes();
}

void mapeditor::_initialise(void)
{
	SystemManager* sysman = Services::get().get_system_manager();

	sysman->create<InputSignalProcessor>(SystemOrdering(0, UpdatePriority::POSTINPUT, 0));
	sysman->create<CameraControllerScript>(SystemOrdering(0, UpdatePriority::POSTINPUT, 1));
	sysman->create<UnitPlacer>(SystemOrdering(0, UpdatePriority::GAMEPLAY, 0));
}