#pragma once

#include <queue>

#include "service.h"
#include "gfx/uniform_buffer_object.h"
#include "gfx/shader_storage_buffer.h"

#define MAX_COMMANDS (1024*1024)
#define MAX_UBOS 16
#define MAX_SSBOS 16

class Service;
class RenderCommand;
class UpdateBufferObjectCommand;
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

	// Submit a list of RenderCommands for the renderer to process when it comes time to rendering
	void submit_render_commands(std::vector<const RenderCommand*>& commands);

	// Executes render commands submitted and any other enabled options
	void render(void);

private:

	int _command_count;
	const RenderCommand* _commands[MAX_COMMANDS];
	UniformBufferObject _ubos[MAX_UBOS];
	ShaderStorageBufferObject _ssbos[MAX_SSBOS];

	// Updates all specified uniform buffers and binds them
	void _update_buffers(const UpdateBufferObjectCommand* command);
};