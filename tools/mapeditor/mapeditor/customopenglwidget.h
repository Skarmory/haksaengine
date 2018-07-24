#pragma once

#include "engine.h"
#include <QOpenGLWidget>

class CustomOpenGLWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	CustomOpenGLWidget(QWidget *parent);
	~CustomOpenGLWidget();

	void set_engine(Engine* engine);

protected:
	
	void paintGL(void) override;

private:
	Engine* _engine;
};
