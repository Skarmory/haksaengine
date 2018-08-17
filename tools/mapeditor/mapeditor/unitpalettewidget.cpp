#include "unitpalettewidget.h"

//#include <experimental/filesystem>

#include "services.h"

#include "variant.h"

#include "editor_state.h"

UnitPaletteWidget::UnitPaletteWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	this->hide();
}

UnitPaletteWidget::~UnitPaletteWidget()
{
}

void UnitPaletteWidget::init(void)
{
	Services::get().get_event_manager()->subscribe("UnitPlaceRequestEvent", this);
	Services::get().get_event_manager()->subscribe("ObjectPlaceRequestEvent", this);
}

void UnitPaletteWidget::set_units(const std::vector<const Blueprint*>& units)
{
	unit_blueprints = units;
}

void UnitPaletteWidget::set_objects(const std::vector<const Blueprint*>& objects)
{
	object_blueprints = objects;
}

void UnitPaletteWidget::update_all(void)
{
	AssetManager* assetman = Services::get().get_asset_manager();

	std::string asset_path = assetman->get_asset_directory_path();

	std::string full_unit_path = asset_path + unit_path;
	std::string full_icon_path = asset_path + icon_path;

	int i = 0;
	int width = 4;

	for (auto blueprint : unit_blueprints)
	{
		const char* filepath = blueprint->get_name();

		std::string filename(filepath + unit_dir.size());

		std::string iconpath = full_icon_path + filename.replace(filename.end() - 3, filename.end(), "png");

		QPushButton* button = new QPushButton(ui.units_group_box);
		QPixmap pixmap(iconpath.c_str());
		QIcon icon(pixmap);

		button->setIcon(icon);
		button->setIconSize(QSize(64, 64));

		button->setMinimumSize(QSize(74, 74));
		button->setMaximumSize(QSize(74, 74));

		button->setCheckable(true);

		button->move(
			10 + (84 * ((i % width))),
			15 + (84 * ((i / width)))
		);

		button->show();

		connect(button, SIGNAL(clicked()), this, SLOT(swap_to_unit_place()));

		units_button_group.addButton(button, i);

		i++;
	}

	i = 0;

	for (auto blueprint : object_blueprints)
	{
		const char* filepath = blueprint->get_name();

		std::string filename(filepath + object_dir.size());

		std::string iconpath = full_icon_path + filename.replace(filename.end() - 3, filename.end(), "png");

		QPushButton* button = new QPushButton(ui.objects_group_box);
		QPixmap pixmap(iconpath.c_str());
		QIcon icon(pixmap);

		button->setIcon(icon);
		button->setIconSize(QSize(64, 64));

		button->setMinimumSize(QSize(74, 74));
		button->setMaximumSize(QSize(74, 74));

		button->setCheckable(true);

		button->move(
			10 + (84 * ((i % width))),
			15 + (84 * ((i / width)))
		);

		button->show();

		connect(button, SIGNAL(clicked()), this, SLOT(swap_to_object_place()));

		object_button_group.addButton(button, i);

		i++;
	}
}

void UnitPaletteWidget::on_event(Event e)
{
	if (e.event_type == "UnitPlaceRequestEvent")
	{
		const Mouse& mouse = Services::get().get_input_manager()->get_mouse();

		Ray ray = Services::get().get_scene_manager()->screen_to_world_ray(mouse.x(), mouse.y());

		glm::vec3 xsect;
		if (Services::get().get_scene_manager()->get_terrain()->intersect(ray, xsect))
		{
			int bprid = units_button_group.checkedId();

			Event e;
			Variant arg;

			e.event_type = "UnitPlaceEvent";

			arg.type = Variant::CONST_POINTER;
			arg.as_const_ptr = static_cast<const void*>(unit_blueprints[bprid]);
			e.arguments.push_back(arg);

			arg.type = Variant::FLOAT;
			arg.as_float = xsect.x;
			e.arguments.push_back(arg);

			arg.as_float = xsect.y;
			e.arguments.push_back(arg);

			arg.as_float = xsect.z;
			e.arguments.push_back(arg);

			Services::get().get_event_manager()->dispatch(e);
		}	
	}
	else if (e.event_type == "ObjectPlaceRequestEvent")
	{
		const Mouse& mouse = Services::get().get_input_manager()->get_mouse();

		Ray ray = Services::get().get_scene_manager()->screen_to_world_ray(mouse.x(), mouse.y());

		glm::vec3 xsect;
		if (Services::get().get_scene_manager()->get_terrain()->intersect(ray, xsect))
		{
			int bprid = object_button_group.checkedId();

			Event e;
			Variant arg;

			e.event_type = "ObjectPlaceEvent";

			arg.type = Variant::CONST_POINTER;
			arg.as_const_ptr = static_cast<const void*>(object_blueprints[bprid]);
			e.arguments.push_back(arg);

			arg.type = Variant::FLOAT;
			arg.as_float = xsect.x;
			e.arguments.push_back(arg);

			arg.as_float = xsect.y;
			e.arguments.push_back(arg);

			arg.as_float = xsect.z;
			e.arguments.push_back(arg);

			Services::get().get_event_manager()->dispatch(e);
		}
	}
}

void UnitPaletteWidget::swap_to_unit_place(void)
{
	EditorState::swap_state(new UnitPlaceState);
}

void UnitPaletteWidget::swap_to_object_place(void)
{
	EditorState::swap_state(new ObjectPlaceState);
}