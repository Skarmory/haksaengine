#pragma once

#include <string>

#include <GL/glew.h>

#include "globals.h"
#include "asset_manager.h"

class Renderer;

/* Wraps a an OpenGL shader program */
class Shader : public Asset
{
	friend class ShaderLoader;
	friend class Renderer;

public:
	Shader(void);
	~Shader(void);

private:
	GLuint _program;

	// Add a shader to this shader program
	void _attach_shader(GLuint shader_id);

	// Link this shader program
	bool _link(void);

	// Make this the active shader program
	void _use(void) const;

	// Get program id
	GLuint _get_program(void) const;
};