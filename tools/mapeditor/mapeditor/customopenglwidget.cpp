#include "customopenglwidget.h"

CustomOpenGLWidget::CustomOpenGLWidget(QWidget *parent)
	: QOpenGLWidget(parent), _engine(nullptr)
{
	setMouseTracking(true);

	QSurfaceFormat format;
	format.setVersion(4, 5);
	format.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);

	this->setFormat(format);

	_input_handler = new QtInputHandler;
}

CustomOpenGLWidget::~CustomOpenGLWidget()
{
}

void CustomOpenGLWidget::set_engine(Engine* engine)
{
	_engine = engine;

	Services::get().set_input_manager(_input_handler);
}

void CustomOpenGLWidget::paintGL(void)
{
	makeCurrent();

	if(_engine)
		_engine->one_frame();
}

void CustomOpenGLWidget::mousePressEvent(QMouseEvent* event)
{
	_input_handler->process_mouse_pressed(event);
}

void CustomOpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
	_input_handler->process_mouse_pressed(event);
}

void CustomOpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
	_input_handler->process_mouse_move(event);
}

void CustomOpenGLWidget::wheelEvent(QWheelEvent* event)
{
	_input_handler->process_mouse_scroll(event);
}