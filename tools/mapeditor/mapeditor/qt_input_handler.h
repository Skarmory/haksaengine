#pragma once

#include <queue>
#include <thread>
#include <mutex>

#include "input/input_manager.h"
#include "input/mouse.h"
#include "variant.h"

#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>



struct QtInputEvent
{
	enum QtInputEventType
	{
		MOUSE_MOVE = 0,
		MOUSE_PRESS = 1,
		MOUSE_RELEASE = 2,
		MOUSE_SCROLL = 3
	};

	QtInputEventType type;
	std::vector<Variant> data;
};

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

	std::mutex mu;

	std::queue<QtInputEvent> events;
};