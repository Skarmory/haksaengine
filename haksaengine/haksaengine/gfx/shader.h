#pragma once

#include <string>

#include <GL/glew.h>

#include "globals.h"
#include "asset_manager.h"

/* Wraps a an OpenGL shader program */
class Shader : public Asset
{
public:
	Shader(void);
	~Shader(void);

	// Add a shader to this shader program
	void attach_shader(GLuint shader_id);

	// Link this shader program
	bool link(void);

	// Make this the active shader program
	void use(void) const;

	// Get program id
	GLuint get_program(void) const;

private:
	GLuint _program;
};