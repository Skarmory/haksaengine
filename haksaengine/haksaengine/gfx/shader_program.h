#pragma once

#include <vector>
#include <GL/glew.h>

#include "globals.h"
#include "gfx/shader.h"

class HAKSAENGINE_API ShaderProgram
{
public:
	ShaderProgram(void);
	~ShaderProgram(void);

	GLuint get_program(void) const;

	void attach_shader(Shader const * const shader);
	bool link(void);
	void use(void);

private:
	GLuint program;
	std::vector<const Shader*> shaders;
};