#include "gfx/shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& filepath, int shader_type)
{
	std::string input;
	std::ifstream file;

	file.exceptions(std::ifstream::badbit);

	try
	{
		std::stringstream stream;

		file.open(filepath);
		stream << file.rdbuf();
		file.close();

		input = stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Failed to read shader file" << std::endl;
	}

	const GLchar* code = input.c_str();

	shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &code, nullptr);
	glCompileShader(shader);

	GLint success;
	GLchar log[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, nullptr, log);
		std::cout << "Failed to compile shader\n" << log << std::endl;
	}
}

Shader::~Shader(void)
{
	glDeleteShader(shader);
}

GLuint Shader::get_shader(void)
{
	return shader;
}

int Shader::get_type(void)
{
	return type;
}