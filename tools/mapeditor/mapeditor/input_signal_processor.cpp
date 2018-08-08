#include "input_signal_processor.h"

#include "editor_state.h"

InputSignalProcessor::InputSignalProcessor(SystemOrdering order) : System(order)
{
}

void InputSignalProcessor::update(float delta)
{
	EditorState::get()->execute();
}

void InputSignalProcessor::on_event(Event e)
{
}