#include "gfx/renderer.h"

#include <GL/glew.h>

#include "gfx/render_command.h"

Renderer::Renderer(void) : _command_count(0)
{
	for (int i = 0; i < MAX_UBOS; i++)
	{
		_ubos[i]._location = i;
	}
}

Renderer::~Renderer(void)
{
	for (int i = 0; i < _command_count; i++)
		delete _commands[i];
}

void Renderer::submit_render_commands(std::vector<const RenderCommand*>& commands)
{
	auto size = commands.size();

	if (_command_count + size > MAX_COMMANDS)
		throw std::runtime_error("Too many rendering commands");

	auto data = commands.data();

	std::memcpy(&_commands[_command_count], data, sizeof(RenderCommand*) * size);

	_command_count += size;

	commands.clear();
}

void Renderer::render(void)
{
	const RenderCommand* command;
	for (int i = 0; i < _command_count; i++)
	{
		command = _commands[i];

		switch (command->_type)
		{
			case RenderCommandType::BindMesh:
			{
				const BindMeshCommand* bm_cmd = static_cast<const BindMeshCommand*>(command);
				bm_cmd->_mesh._bind();
				break;
			}

			case RenderCommandType::MakeTextureHandlesResident:
			{
				const MakeTextureHandlesResidentCommand* mthr_cmd = static_cast<const MakeTextureHandlesResidentCommand*>(command);
				for (auto handle : mthr_cmd->_handles)
					glMakeTextureHandleResidentARB(handle);
				break;
			}

			case RenderCommandType::UpdateUniforms:
			{
				const UpdateUniformsCommand* uu_cmd = static_cast<const UpdateUniformsCommand*>(command);
				_update_uniform_buffers(uu_cmd);
				break;
			}

			case RenderCommandType::UseShader:
			{
				const UseShaderCommand* us_cmd = static_cast<const UseShaderCommand*>(command);
				us_cmd->_shader.use();
				break;
			}

			case RenderCommandType::DrawIndexed:
			{
				const DrawIndexedCommand* di_cmd = static_cast<const DrawIndexedCommand*>(command);

				int primitive_type = di_cmd->_primitive_type == PrimitiveType::Triangles ? GL_TRIANGLES : di_cmd->_primitive_type == PrimitiveType::Lines ? GL_LINES : GL_POINTS;

				glDrawElements(primitive_type, di_cmd->_index_count, GL_UNSIGNED_INT, (const void*)(di_cmd->_offset * sizeof(GLuint)));

				break;
			}
		}

		delete command;
	}

	_command_count = 0;
}

void Renderer::_update_uniform_buffers(const UpdateUniformsCommand* command)
{
	for (auto uniform : command->_uniforms)
	{
		unsigned int location = uniform->get_location();

		if (location > MAX_UBOS)
			throw std::runtime_error("Uniform location not in range");

		if (!_ubos[location]._initialised)
		{
			_ubos[location].initialise();
		}

		_ubos[location].update(uniform);

		_ubos[location].bind();
	}
}