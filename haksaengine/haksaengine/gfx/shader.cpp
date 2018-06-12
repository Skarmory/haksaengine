#include "gfx/shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(void)
{
	_program = glCreateProgram();
}

Shader::~Shader(void)
{
	glDeleteProgram(_program);
}

void Shader::attach_shader(GLuint shader_id)
{
	glAttachShader(_program, shader_id);
}

bool Shader::link(void)
{
	// Link shader program
	glLinkProgram(_program);

	GLint success;
	GLchar log[512];

	// Check link status
	glGetProgramiv(_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		// Print error out
		glGetProgramInfoLog(_program, 512, NULL, log);
		std::cout << "Failed to link shader program\n" << log << std::endl;
	}

	return success;
}

void Shader::use(void)
{
	glUseProgram(_program);
}