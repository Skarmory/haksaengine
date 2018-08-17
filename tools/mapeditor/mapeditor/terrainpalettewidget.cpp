#include "terrainpalettewidget.h"

#include <QLayout>

#include "services.h"

#include "scene/terrain.h"

#include "editor_state.h"

TerrainPaletteWidget::TerrainPaletteWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.lower_button->setCheckable(true);
	ui.raise_button->setCheckable(true);

	connect(ui.lower_button, SIGNAL(clicked()), this, SLOT(swap_to_height_deform()));
	connect(ui.raise_button, SIGNAL(clicked()), this, SLOT(swap_to_height_deform()));

	_height_deform_button_group.addButton(ui.lower_button, HeightDeform::Lower);
	_height_deform_button_group.addButton(ui.raise_button, HeightDeform::Upper);
}

TerrainPaletteWidget::~TerrainPaletteWidget()
{
	for (auto button : _texture_buttons)
		delete button;
}

void TerrainPaletteWidget::init(void)
{
	Services::get().get_event_manager()->subscribe("PaintTextureEvent", this);
	Services::get().get_event_manager()->subscribe("HeightDeformEvent", this);
}

void TerrainPaletteWidget::update_textures(void)
{
	std::string tex_path;

	const Tileset& tileset = Services::get().get_scene_manager()->get_terrain()->get_tileset();

	int i = 0;
	int width = 4;
	while (!(tex_path = tileset.texture_path(i)).empty())
	{
		QPushButton* button = new QPushButton(ui.textures_group_box);
		QPixmap pixmap(tex_path.c_str());
		QIcon icon(pixmap);

		button->setIcon(icon);
		button->setIconSize(QSize(32, 32));

		button->setMinimumSize(QSize(40, 40));
		button->setMaximumSize(QSize(40, 40));

		button->setCheckable(true);
		
		button->move(
			10 + (50 * ((i % width))),		
			15 + (50 * ((i / width)))
		);

		button->show();

		connect(button, SIGNAL(clicked()), this, SLOT(swap_to_texture_paint()));

		_textures_button_group.addButton(button, i);

		i++;
	}
}

void TerrainPaletteWidget::on_event(Event e)
{
	if (e.event_type == "PaintTextureEvent")
	{
		const Mouse& mouse = Services::get().get_input_manager()->get_mouse();

		Ray ray = Services::get().get_scene_manager()->screen_to_world_ray(mouse.x(), mouse.y());

		Terrain* terrain = Services::get().get_scene_manager()->get_terrain();

		TerrainVertexData* vertex;

		if (vertex = terrain->intersect(ray))
		{
			int tid = _textures_button_group.checkedId();

			if (tid > -1 && vertex->texture != tid)
			{
				vertex->texture = _textures_button_group.checkedId();

				terrain->update(vertex);
			}
		}	
	}
	else if (e.event_type == "HeightDeformEvent")
	{
		const Mouse& mouse = Services::get().get_input_manager()->get_mouse();

		Ray ray = Services::get().get_scene_manager()->screen_to_world_ray(mouse.x(), mouse.y());

		Terrain* terrain = Services::get().get_scene_manager()->get_terrain();

		TerrainVertexData* vertex;

		if (vertex = terrain->intersect(ray))
		{
			switch (_height_deform_button_group.checkedId())
			{
				default:
				case HeightDeform::Lower:
				{
					vertex->position.y -= 1.0f;
					if (vertex->position.y < -128.0f)
						vertex->position.y = -128.0f;
					break;
				}

				case HeightDeform::Upper:
				{
					vertex->position.y += 1.0f;
					if (vertex->position.y > 128.0f)
						vertex->position.y = 128.0f;
					break;
				}
			}

			terrain->update(vertex);
		}
	}
}

void TerrainPaletteWidget::swap_to_texture_paint(void)
{
	EditorState::swap_state(new TexturePaintState);
}

void TerrainPaletteWidget::swap_to_height_deform(void)
{
	EditorState::swap_state(new HeightDeformState);
}
