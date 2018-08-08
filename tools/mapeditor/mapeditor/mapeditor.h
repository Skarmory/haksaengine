#pragma once

#include "engine.h"

#include <QtWidgets/QMainWindow>
#include <QtCore/QTimer>
#include "ui_mapeditor.h"
#include "palettewindow.h"

class mapeditor : public QMainWindow
{
	Q_OBJECT

public:
	mapeditor(QWidget *parent = Q_NULLPTR);
	~mapeditor(void);

private slots:

	void main_loop(void);

private:
	Ui::mapeditorClass ui;

	PaletteWindow palette;

	QTimer* _main_loop_timer;
	Engine* _engine;

	void _setup_default_scene(void);
	void _initialise(void);
};
