#include "mapeditor.h"

mapeditor::mapeditor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	_main_loop_timer = new QTimer(this);
	connect(_main_loop_timer, SIGNAL(timeout()), this, SLOT(main_loop()));

	_engine = new Engine(EngineMode::Editor);
}


void mapeditor::main_loop(void)
{
	
}