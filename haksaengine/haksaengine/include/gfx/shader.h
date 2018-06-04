#pragma once

#include <string>

#include <GL/glew.h>

#include "globals.h"

/* Wraps a an OpenGL shader */
class HAKSAENGINE_API Shader
{
public:
	// Read, create, and compile shader from file path
	Shader(const std::string& filepath, int shader_type);
	~Shader(void);

	GLuint get_shader(void) const;
	int get_type(void) const;

private:
	GLuint shader;
	int type;
};