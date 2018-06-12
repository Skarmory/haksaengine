#include "shader_loader.h"

#include <iostream>
#include <fstream>
#include <sstream>

ShaderLoader::ShaderLoader(const std::string& directory) : Loader(directory, ".shader")
{
}

Shader* ShaderLoader::load(const std::string& name)
{
	Shader* shader = new Shader;

	std::string path = get_path(name);

	// Open stream and read into stringstream
	std::ifstream fs(path);
	std::stringstream ss;
	ss << fs.rdbuf();
	fs.close();

	// Get shader file code as string
	std::string shader_source_string = ss.str();

	// Compile and link shaders
	process_shader_source(shader_source_string, shader);

	return shader;
}

void ShaderLoader::process_shader_source(const std::string& source, Shader* shader)
{
	std::vector<GLuint> shaders;
	shaders.reserve(5);

	// Check for vertex shader
	if (source.find("#ifdef VERTEX") != std::string::npos)
		shaders.push_back(create_shader(version_string + "#define VERTEX\n" + source, GL_VERTEX_SHADER));
	
	// Check for fragment shader
	if (source.find("#ifdef FRAGMENT") != std::string::npos)
		shaders.push_back(create_shader(version_string + "#define FRAGMENT\n" + source, GL_FRAGMENT_SHADER));

	for (GLuint s : shaders)
		shader->attach_shader(s);

	// Link shader program
	shader->link();

	// Program linked, now we can just delete the shaders
	for (GLuint s : shaders)
		glDeleteShader(s);
}

GLuint ShaderLoader::create_shader(const std::string& source, int shader_type)
{
	GLuint shader;
	const GLchar* code = source.c_str();
	GLint success;
	GLchar log[512];

	// Create shader, specify source, and compile it
	shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &code, nullptr);
	glCompileShader(shader);

	// Check status of shader compilation
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		// Write out errors
		glGetShaderInfoLog(shader, 512, nullptr, log);
		std::cout << "Failed to compile shader" << std::endl << log << std::endl;
	}

	return shader;
}