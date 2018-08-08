#include "palettewindow.h"

PaletteWindow::PaletteWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	terrain_widget = new TerrainPaletteWidget(this);
	terrain_widget->move(0, 40);

	current_widget = terrain_widget;

	current_widget->show();

	connect(ui.palette_combo_box, SIGNAL(currentIndexChanged(int)), this, SLOT(_palette_swapped(int)));
}

PaletteWindow::~PaletteWindow()
{
	delete terrain_widget;
}

void PaletteWindow::_palette_swapped(int palette)
{
	palette_type = static_cast<PaletteType>(palette);

	current_widget->hide();

	switch (palette_type)
	{
	case PaletteType::Terrain:
	{
		current_widget = terrain_widget;
		break;
	}

	case PaletteType::Unit:
	{
		break;
	}

	default:
		current_widget = terrain_widget;
		break;
	}

	current_widget->show();
}

void PaletteWindow::init_palettes(void)
{
	terrain_widget->init();
}

void PaletteWindow::update_palettes(void)
{
	terrain_widget->update_textures();
}
