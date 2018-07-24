#include "customopenglwidget.h"

CustomOpenGLWidget::CustomOpenGLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{

	QSurfaceFormat format;
	format.setVersion(4, 5);
	format.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);

	this->setFormat(format);
}

CustomOpenGLWidget::~CustomOpenGLWidget()
{
}

void CustomOpenGLWidget::set_engine(Engine* engine)
{
	_engine = engine;
}

void CustomOpenGLWidget::paintGL(void)
{
	makeCurrent();

	_engine->one_frame();
}