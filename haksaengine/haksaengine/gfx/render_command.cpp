#include "gfx/render_command.h"

RenderCommand::RenderCommand(RenderCommandType type) : _type(type)
{
}

BindMeshCommand::BindMeshCommand(const Mesh& mesh) : _mesh(mesh), RenderCommand(RenderCommandType::BindMesh)
{
}

MakeTextureHandlesResidentCommand::MakeTextureHandlesResidentCommand(void) : RenderCommand(RenderCommandType::MakeTextureHandlesResident)
{
}

void MakeTextureHandlesResidentCommand::add_texture_handle(BindlessTextureHandle handle)
{
	_handles.push_back(handle);
}

UseShaderCommand::UseShaderCommand(const Shader& shader) : _shader(shader), RenderCommand(RenderCommandType::UseShader)
{
}

//UpdateUniformCommand::UpdateUniformCommand(const std::vector<const BaseUniform*> uniforms) : _uniforms(uniforms), RenderCommand(RenderCommandType::UpdateUniforms)
//{
//}

UpdateUniformsCommand::UpdateUniformsCommand(void) : RenderCommand(RenderCommandType::UpdateUniforms)
{
}

UpdateUniformsCommand::~UpdateUniformsCommand(void)
{
	for (auto uniform : _uniforms)
		delete uniform;
}

void UpdateUniformsCommand::add_uniform(const BaseUniform* uniform)
{
	_uniforms.push_back(uniform);
}

DrawIndexedCommand::DrawIndexedCommand(PrimitiveType primitive_type, unsigned int index_count, unsigned int offset)
	: _primitive_type(primitive_type), _index_count(index_count), _offset(offset),
	RenderCommand(RenderCommandType::DrawIndexed)
{
}

DrawIndexedInstancedCommand::DrawIndexedInstancedCommand(PrimitiveType primitive_type, unsigned int index_count, unsigned int offset, unsigned int instance_count) 
	: _primitive_type(primitive_type), _index_count(index_count), _offset(offset), _instance_count(instance_count),
	RenderCommand(RenderCommandType::DrawIndexedInstanced)
{
}