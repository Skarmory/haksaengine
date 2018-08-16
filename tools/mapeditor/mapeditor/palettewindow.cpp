#include "palettewindow.h"

#include "editor_state.h"

PaletteWindow::PaletteWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	terrain_widget = new TerrainPaletteWidget(this);
	terrain_widget->move(0, 40);

	current_widget = terrain_widget;

	current_widget->show();

	connect(ui.palette_combo_box, SIGNAL(currentIndexChanged(int)), this, SLOT(_palette_swapped(int)));

	EditorState::swap_state(new TexturePaintState);
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
	default:
	case PaletteType::Terrain:
	{
		current_widget = terrain_widget;
		EditorState::swap_state(new TexturePaintState);
		break;
	}

	case PaletteType::Unit:
	{
		break;
	}
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
