#pragma once

#include <queue>

#include "service.h"
#include "gfx/uniform_buffer_object.h"

#define MAX_COMMANDS (1024*1024)
#define MAX_UBOS 16

class Service;
class RenderCommand;
class UpdateUniformsCommand;
class UniformBufferObject;

// Encapsulates all OpenGL drawing logic.
// RenderCommand queues need to be submitted to it
class Renderer : public Service
{
public:

	Renderer(void);
	~Renderer(void);

	// No copying
	Renderer(const Renderer& other) = delete;
	Renderer& operator=(const Renderer& other) = delete;

	// No moving
	Renderer(const Renderer&& other) = delete;
	Renderer& operator=(const Renderer&& other) = delete;

	// Adds a RenderLogicSystem derived class to 
	void submit_render_commands(std::vector<const RenderCommand*>& commands);

	// Executes render commands submitted and any other enabled options
	void render(void);

private:

	int _command_count;
	const RenderCommand* _commands[MAX_COMMANDS];
	UniformBufferObject _ubos[MAX_UBOS];

	void _update_uniform_buffers(const UpdateUniformsCommand* command);
};