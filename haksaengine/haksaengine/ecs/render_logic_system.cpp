#include "render_logic_system.h"

#include "services.h"

RenderLogicSystem::RenderLogicSystem(SystemOrdering order) : System(order)
{
}

RenderLogicSystem::~RenderLogicSystem(void)
{
	if(_current_uniform_command)
		delete _current_uniform_command;
}

void RenderLogicSystem::bind_mesh(const Mesh& mesh)
{
	BindMeshCommand* command = new BindMeshCommand(mesh);

	_command_queue.push_back(command);
}

void RenderLogicSystem::bind_texture(const Texture& texture)
{
	if (!_current_texture_command)
		_current_texture_command = new MakeTextureHandlesResidentCommand;

	_current_texture_command->add_texture_handle(texture.get_handle());
}

void RenderLogicSystem::use_shader(const Shader& shader)
{
	UseShaderCommand* command = new UseShaderCommand(shader);

	_command_queue.push_back(command);
}

void RenderLogicSystem::draw_indexed(PrimitiveType primitive_type, unsigned int index_count, unsigned int offset)
{
	DrawIndexedCommand* command = new DrawIndexedCommand(primitive_type, index_count, offset);

	_command_queue.push_back(_current_texture_command);
	_command_queue.push_back(_current_uniform_command);
	_command_queue.push_back(command);

	_current_texture_command = nullptr;
	_current_uniform_command = nullptr;
}

void RenderLogicSystem::submit_commands(void)
{
	Services::get<Renderer>()->submit_render_commands(_command_queue);
}