#pragma once

#include <vector>

#include "gfx/uniform.h"
#include "gfx/mesh.h"
#include "gfx/shader.h"
#include "gfx/texture.h"

class Renderer;

enum RenderCommandType
{
	BindMesh = 0,
	MakeTextureHandlesResident = 1,
	UseShader = 2,
	UpdateUniforms = 3,
	DrawIndexed = 4,
	DrawIndexedInstanced = 5
};

enum PrimitiveType
{
	Points = 0,
	Lines = 1,
	Triangles = 2
};

// Base class all render commands must inherit from
class RenderCommand
{
public:
	RenderCommand(RenderCommandType type);
	virtual ~RenderCommand(void) = default;

private:
	RenderCommandType _type;

	friend class Renderer;
};

// Contains mesh data for the Renderer to bind
class BindMeshCommand : public RenderCommand
{
public:
	explicit BindMeshCommand(const Mesh& mesh);

private:
	const Mesh& _mesh;

	friend class Renderer;
};

// Contains a vector of bindless texture handle that will need to be used this draw
class MakeTextureHandlesResidentCommand : public RenderCommand
{
public:
	MakeTextureHandlesResidentCommand(void);

	void add_texture_handle(BindlessTextureHandle handle);

private:
	std::vector<BindlessTextureHandle> _handles;

	friend class Renderer;
};

// Contains a shader program for the Renderer to use
class UseShaderCommand : public RenderCommand
{
public:
	explicit UseShaderCommand(const Shader& shader);

private:
	const Shader& _shader;

	friend class Renderer;
};

// Contains uniform data for the Renderer update a uniform buffer
// Uniform pointers given to this are considered owned by this command now
class UpdateUniformsCommand : public RenderCommand
{
public:
	//explicit UpdateUniformCommand(const std::vector<const BaseUniform*> uniforms);

	UpdateUniformsCommand(void);
	~UpdateUniformsCommand(void);

	void add_uniform(const BaseUniform* uniform);

private:
	/*const BaseUniform* _uniform;*/
	std::vector<const BaseUniform*> _uniforms;

	friend class Renderer;
};

// Contains indexed draw call arguments
class DrawIndexedCommand : public RenderCommand
{
public:
	explicit DrawIndexedCommand(PrimitiveType primitive_type, unsigned int index_count, unsigned int offset);

private:
	PrimitiveType _primitive_type;
	unsigned int _index_count;
	unsigned int _offset;

	friend class Renderer;
};

class DrawIndexedInstancedCommand : public RenderCommand
{
	friend class Renderer;

public:
	explicit DrawIndexedInstancedCommand(PrimitiveType primitive_type, unsigned int index_count, unsigned int offset, unsigned int instance_count);

private:
	PrimitiveType _primitive_type;
	unsigned int _index_count;
	unsigned int _offset;
	unsigned int _instance_count;
};