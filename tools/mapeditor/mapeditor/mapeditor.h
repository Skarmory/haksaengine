#pragma once

#include "engine.h"

#include <atomic>

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
	
	void initialise(void);
	void main_loop(void);

private:
	Ui::mapeditorClass ui;

	std::atomic_bool working;

	PaletteWindow palette;

	QTimer* _main_loop_timer, *_init_timer;
	Engine* _engine;

	// Load all the assets the editor will need here
	// Switching to different widgets breaks the OpenGL state, causing everything to fail
	void _load_assets(void);

	// Create default map
	void _setup_default_scene(void);

	// Adds editor systems to the engine
	void _initialise(void);
};
