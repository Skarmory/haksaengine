#pragma once

#include <vector>

#include "gfx/uniform.h"
#include "gfx/mesh.h"
#include "gfx/terrain_mesh.h"
#include "gfx/shader.h"
#include "gfx/texture.h"

class Renderer;

enum RenderCommandType
{
	BindMesh = 0,
	BindTerrainMesh = 1,
	MakeTextureHandlesResident = 2,
	UseShader = 3,
	UpdateBuffers = 4,
	DrawIndexed = 5,
	DrawIndexedInstanced = 6
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
	friend class Renderer;

public:

	RenderCommand(RenderCommandType type);
	virtual ~RenderCommand(void) = default;

private:

	RenderCommandType _type;
};

// Contains mesh data for the Renderer to bind
class BindMeshCommand : public RenderCommand
{
	friend class Renderer;

public:

	explicit BindMeshCommand(const Mesh& mesh);

private:

	const Mesh& _mesh;
};

// Contains terrain mesh data for the Renderer to bind
class BindTerrainMeshCommand : public RenderCommand
{
	friend class Renderer;

public:

	explicit BindTerrainMeshCommand(const TerrainMesh& mesh);

private:

	const TerrainMesh& _mesh;
};


// Contains a vector of bindless texture handle that will need to be used this draw
class MakeTextureHandlesResidentCommand : public RenderCommand
{
	friend class Renderer;

public:

	MakeTextureHandlesResidentCommand(void);

	void add_texture_handle(BindlessTextureHandle handle);

private:

	std::vector<BindlessTextureHandle> _handles;
};

// Contains a shader program for the Renderer to use
class UseShaderCommand : public RenderCommand
{
	friend class Renderer;

public:

	explicit UseShaderCommand(const Shader& shader);

private:

	const Shader& _shader;
};

// Contains uniform data for the Renderer update a uniform buffer
// Uniform pointers given to this are considered owned by this command now
class UpdateBufferObjectCommand : public RenderCommand
{
	friend class Renderer;

public:

	UpdateBufferObjectCommand(void);
	~UpdateBufferObjectCommand(void);

	void add_uniform(const BaseBuffer* uniform);
	void add_ssbo(const BaseBuffer* uniform);

private:

	std::vector<const BaseBuffer*> _uniforms;
	std::vector<const BaseBuffer*> _ssbos;
};

// Contains indexed draw call arguments
class DrawIndexedCommand : public RenderCommand
{
	friend class Renderer;

public:

	explicit DrawIndexedCommand(PrimitiveType primitive_type, unsigned int index_count, unsigned int offset);

private:

	PrimitiveType _primitive_type;
	unsigned int _index_count;
	unsigned int _offset;
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