#include "palettewindow.h"

#include "editor_state.h"

PaletteWindow::PaletteWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	terrain_widget = new TerrainPaletteWidget(this);
	terrain_widget->move(0, 40);

	units_widget = new UnitPaletteWidget(this);
	units_widget->move(0, 40);

	current_widget = terrain_widget;

	connect(ui.palette_combo_box, SIGNAL(currentIndexChanged(int)), this, SLOT(_palette_swapped(int)));
}

PaletteWindow::~PaletteWindow()
{
	delete units_widget;
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
		EditorState::swap_state(new TexturePaintState);
		break;
	}

	case PaletteType::Unit:
	{
		current_widget = units_widget;
		break;
	}
	}

	current_widget->show();
}

void PaletteWindow::init_palettes(void)
{
	terrain_widget->init();
	units_widget->init();

	_palette_swapped((int)PaletteType::Terrain);
}

void PaletteWindow::update_palettes(void)
{
	terrain_widget->update_textures();
	units_widget->update_all();
}

QWidget* PaletteWindow::get_palette(PaletteType type)
{
	switch ((int)type)
	{
	case (int)PaletteType::Terrain:
		{ return terrain_widget; }
	case (int)PaletteType::Unit:
		{ return units_widget; }
	}

	return nullptr;
}
