#pragma once

#include "input/input_manager.h"
#include "input/mouse.h"

#include <QMouseEvent>
#include <QWheelEvent>

class QtInputHandler : public InputManager
{
public:

	QtInputHandler(void);

	void update(void) override;

	void process_key(void);
	void process_mouse_move(QMouseEvent* event);
	void process_mouse_pressed(QMouseEvent* event);
	void process_mouse_released(QMouseEvent* event);
	void process_mouse_scroll(QWheelEvent* event);

private:

	MouseButtonType _qt_mouse_button_convert(Qt::MouseButton button);

	float _scroll;
};