#include "shader_program.h"

#include <iostream>

ShaderProgram::ShaderProgram(void)
{
	program = glCreateProgram();
}

ShaderProgram::~ShaderProgram(void)
{
	glDeleteProgram(program);
}

GLuint ShaderProgram::get_program(void) const
{
	return program;
}

void ShaderProgram::attach_shader(const Shader* shader)
{
	shaders.push_back(shader);

	glAttachShader(program, shader->get_shader());
}

bool ShaderProgram::link(void)
{
	glLinkProgram(program);

	GLint success;
	GLchar log[512];

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, log);
		std::cout << "Failed to link shader program\n" << log << std::endl;
	}

	return success;
}

void ShaderProgram::use(void)
{
	glUseProgram(program);
}