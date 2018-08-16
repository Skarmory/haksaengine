#pragma once

#include <QWidget>
#include <QPushButton>
#include "ui_terrainpalettewidget.h"

#include "event/event_handler.h"

class TerrainPaletteWidget : public QWidget, public EventHandler
{
	Q_OBJECT

public:
	TerrainPaletteWidget(QWidget *parent = Q_NULLPTR);
	~TerrainPaletteWidget();

	void init(void);
	void update_textures(void);
	void on_event(Event e) override;

private slots:
	void swap_to_texture_paint(void);
	void swap_to_height_deform(void);

private:
	Ui::TerrainPaletteWidget ui;

	QButtonGroup _textures_button_group;
	QButtonGroup _height_deform_button_group;
	std::vector<QPushButton*> _texture_buttons;

	enum HeightDeform
	{
		Lower = 0,
		Upper = 1
	};
};
