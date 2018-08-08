#pragma once

#include <QMainWindow>
#include "ui_palettewindow.h"

#include "terrainpalettewidget.h"

enum class PaletteType
{
	Terrain = 0,
	Unit = 1
};

class PaletteWindow : public QMainWindow
{
	Q_OBJECT

public:
	PaletteWindow(QWidget *parent = Q_NULLPTR);
	~PaletteWindow();

	void init_palettes(void);
	void update_palettes(void);

private slots:
	void _palette_swapped(int palette);


private:
	Ui::PaletteWindow ui;

	PaletteType palette_type;

	QWidget* current_widget;
	TerrainPaletteWidget* terrain_widget;
};
