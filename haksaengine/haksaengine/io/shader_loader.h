#pragma once

#include <string>

#include "loader.h"
#include "gfx/shader.h"

/* Loader for shader files */
class ShaderLoader : public Loader
{
public:
	ShaderLoader(const std::string& directory);
	Shader* load(const std::string& name) override;

private:
	// Parses shader file source and creates a shader program
	void process_shader_source(std::string& source, Shader* shader);

	// Compile and return a shader
	GLuint create_shader(const std::string& source, int shader_type);

	const std::string version_string = "#version 450 core\n";
};