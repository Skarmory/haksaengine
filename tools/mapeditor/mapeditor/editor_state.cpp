#include "editor_state.h"

#include "services.h"

EditorState* EditorState::current = nullptr;

void TexturePaintState::execute(void)
{
	const Mouse& mouse = Services::get().get_input_manager()->get_mouse();

	if (mouse.pressed(MouseButtonType::Button_1))
	{
		Event paint_tex_event;
		paint_tex_event.event_type = "PaintTextureEvent";

		Services::get().get_event_manager()->dispatch(paint_tex_event);
	}
	else if (mouse.pressed(MouseButtonType::Button_2) || mouse.scroll() != 0.0f)
	{
		Event move_camera;
		move_camera.event_type = "MoveCameraEvent";

		Services::get().get_event_manager()->dispatch(move_camera);
	}
}

void HeightDeformState::execute(void)
{
	const Mouse& mouse = Services::get().get_input_manager()->get_mouse();

	if (mouse.pressed(MouseButtonType::Button_1))
	{
		Event paint_tex_event;
		paint_tex_event.event_type = "HeightDeformEvent";

		Services::get().get_event_manager()->dispatch(paint_tex_event);
	}
	else if (mouse.pressed(MouseButtonType::Button_2) || mouse.scroll() != 0.0f)
	{
		Event move_camera;
		move_camera.event_type = "MoveCameraEvent";

		Services::get().get_event_manager()->dispatch(move_camera);
	}
}