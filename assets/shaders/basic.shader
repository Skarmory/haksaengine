
#ifdef VERTEX

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
}

#endif

#ifdef FRAGMENT

layout (location = 0) in vec3 position;

layout (location = 0) out vec4 colour;

void main()
{
	colour = vec4(1.0, 1.0, 1.0, 1.0);
}

#endif
