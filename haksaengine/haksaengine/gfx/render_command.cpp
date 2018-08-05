#include "gfx/render_command.h"

RenderCommand::RenderCommand(RenderCommandType type) : _type(type)
{
}

BindMeshCommand::BindMeshCommand(const Mesh& mesh) : _mesh(mesh), RenderCommand(RenderCommandType::BindMesh)
{
}

BindTerrainMeshCommand::BindTerrainMeshCommand(const TerrainMesh& mesh) : RenderCommand(RenderCommandType::BindTerrainMesh), _mesh(mesh)
{
}

BindTexturesCommand::BindTexturesCommand(void) : RenderCommand(RenderCommandType::BindTextures)
{
}

void BindTexturesCommand::add_texture_handle(BindlessTextureHandle handle)
{
	_handles.push_back(handle);
}

void BindTexturesCommand::bind_texture(unsigned int binding, const Texture& texture)
{
	_textures.push_back({ binding, texture });
}

void BindTexturesCommand::bind_texture(unsigned int binding, const ArrayTexture& texture)
{
	_array_textures.push_back({ binding, texture });
}

UseShaderCommand::UseShaderCommand(const Shader& shader) : _shader(shader), RenderCommand(RenderCommandType::UseShader)
{
}

UpdateBufferObjectCommand::UpdateBufferObjectCommand(void) : RenderCommand(RenderCommandType::UpdateBuffers)
{
}

UpdateBufferObjectCommand::~UpdateBufferObjectCommand(void)
{
	for (auto uniform : _uniforms)
		delete uniform;

	for (auto ssbo : _ssbos)
		delete ssbo;
}

void UpdateBufferObjectCommand::add_uniform(const BaseBuffer* uniform)
{
	_uniforms.push_back(uniform);
}

void UpdateBufferObjectCommand::add_ssbo(const BaseBuffer* uniform)
{
	_ssbos.push_back(uniform);
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