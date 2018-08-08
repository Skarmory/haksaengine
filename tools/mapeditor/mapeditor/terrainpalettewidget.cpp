#include "terrainpalettewidget.h"

#include <QLayout>

#include "services.h"

#include "scene/terrain.h"

TerrainPaletteWidget::TerrainPaletteWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	_textures_button_group = new QButtonGroup;
}

TerrainPaletteWidget::~TerrainPaletteWidget()
{
}

void TerrainPaletteWidget::init(void)
{
	Services::get().get_event_manager()->subscribe("PaintTextureEvent", this);
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

		_textures_button_group->addButton(button, i);

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
			int tid = _textures_button_group->checkedId();

			if (tid > -1)
			{
				vertex->texture = _textures_button_group->checkedId();

				terrain->update(vertex);
			}
		}	
	}
}
