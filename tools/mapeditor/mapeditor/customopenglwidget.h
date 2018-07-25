#pragma once

#include "engine.h"
#include "qt_input_handler.h"
#include <QOpenGLWidget>
#include <QMouseEvent>

class CustomOpenGLWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	CustomOpenGLWidget(QWidget *parent);
	~CustomOpenGLWidget();

	void set_engine(Engine* engine);

protected:
	
	void paintGL(void) override;

private slots:

	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;

private:

	Engine* _engine;
	QtInputHandler* _input_handler;
};
