
#ifdef VERTEX

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

void main()
{
	gl_Position = vec4(position, 1.0);
}

#endif

#ifdef FRAGMENT

layout (location = 0) in vec3 position;

layout (location = 0) out vec4 colour;

void main()
{
	colour = vec4(1, 1, 1, 1);
}

#endif
