#pragma once

#include "engine.h"

#include <QtWidgets/QMainWindow>
#include <QtCore/QTimer>
#include "ui_mapeditor.h"



class mapeditor : public QMainWindow
{
	Q_OBJECT

public:
	mapeditor(QWidget *parent = Q_NULLPTR);

private slots:

	void main_loop(void);

private:
	Ui::mapeditorClass ui;

	QTimer* _main_loop_timer;
	Engine* _engine;
};
