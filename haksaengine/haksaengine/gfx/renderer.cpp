#include "gfx/renderer.h"

#include <GL/glew.h>

#include "gfx/render_command.h"
#include "gfx/uniform_data.h"

Renderer::Renderer(void) : _command_count(0)
{
	for (int i = 0; i < MAX_UBOS; i++)
	{
		_ubos[i]._location = i;
		_ssbos[i]._location = i;
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
	// DEBUG: This needs to be gotten from the actual Scene
	glm::vec3 sun_dir = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));

	SceneData scene_data;
	scene_data.sun_colour = glm::vec3(1.0, 1.0, 1.0);
	scene_data.sun_direction = glm::vec4(sun_dir, 1.0f);

	// Scene uniform should only need to be updated once
	UpdateBufferObjectCommand scene_uniform_cmd;
	scene_uniform_cmd.add_uniform(new Buffer<SceneData>(SCENE_UNIFORM_BIND_POINT, &scene_data));
	_update_buffers(&scene_uniform_cmd);
	// DEBUG

	// Go through all the commands submitted this frame and process them
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

			case RenderCommandType::BindTerrainMesh:
			{
				const BindTerrainMeshCommand* btm_cmd = static_cast<const BindTerrainMeshCommand*>(command);
				btm_cmd->_mesh._bind();
				break;
			}

			case RenderCommandType::BindTextures:
			{
				const BindTexturesCommand* mthr_cmd = static_cast<const BindTexturesCommand*>(command);
				for (auto handle : mthr_cmd->_handles)
					glMakeTextureHandleResidentARB(handle);

				for (auto& pair : mthr_cmd->_textures)
					pair.second.bind(pair.first);

				for (auto& pair : mthr_cmd->_array_textures)
					pair.second.bind(pair.first);

				break;
			}

			case RenderCommandType::UpdateBuffers:
			{
				const UpdateBufferObjectCommand* uu_cmd = static_cast<const UpdateBufferObjectCommand*>(command);
				_update_buffers(uu_cmd);
				break;
			}

			case RenderCommandType::UseShader:
			{
				const UseShaderCommand* us_cmd = static_cast<const UseShaderCommand*>(command);
				us_cmd->_shader._use();
				break;
			}

			case RenderCommandType::DrawArrays:
			{
				const DrawCommand* d_cmd = static_cast<const DrawCommand*>(command);

				int primitive_type = d_cmd->_primitive_type == PrimitiveType::Triangles ? GL_TRIANGLES : d_cmd->_primitive_type == PrimitiveType::Lines ? GL_LINES : GL_POINTS;

				glDrawArrays(primitive_type, d_cmd->_offset, d_cmd->_count);

				break;
			}

			case RenderCommandType::DrawIndexed:
			{
				const DrawIndexedCommand* di_cmd = static_cast<const DrawIndexedCommand*>(command);

				int primitive_type = di_cmd->_primitive_type == PrimitiveType::Triangles ? GL_TRIANGLES : di_cmd->_primitive_type == PrimitiveType::Lines ? GL_LINES : GL_POINTS;

				glDrawElements(primitive_type, di_cmd->_index_count, GL_UNSIGNED_INT, (const void*)(di_cmd->_offset * sizeof(GLuint)));

				break;
			}
			case RenderCommandType::DrawIndexedInstanced:
			{
				const DrawIndexedInstancedCommand* dii_cmd = static_cast<const DrawIndexedInstancedCommand*>(command);

				int primitive_type = dii_cmd->_primitive_type == PrimitiveType::Triangles ? GL_TRIANGLES : dii_cmd->_primitive_type == PrimitiveType::Lines ? GL_LINES : GL_POINTS;

				glDrawElementsInstanced(primitive_type, dii_cmd->_index_count, GL_UNSIGNED_INT, (const void*)(dii_cmd->_offset * sizeof(GLuint)), dii_cmd->_instance_count);
				
				break;
			}

			default:
				throw std::runtime_error("Unknown render command specified");
		}

		delete command;
	}

	_command_count = 0;
}

void Renderer::_update_buffers(const UpdateBufferObjectCommand* command)
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

		_ubos[location].update(uniform->get_data(), uniform->get_size());

		_ubos[location].bind();
	}

	for (auto ssbo : command->_ssbos)
	{
		unsigned int location = ssbo->get_location();

		if (location > MAX_SSBOS)
			throw std::runtime_error("SSBO location not in range");

		if (!_ssbos[location]._initialised)
		{
			_ssbos[location].initialise();
		}

		_ssbos[location].update(ssbo->get_data(), ssbo->get_size());

		_ssbos[location].bind();
	}
}