
#ifdef VERTEX

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;

layout (location = 0) out vec3 out_position;
layout (location = 1) out vec3 out_normal;
layout (location = 2) out vec2 out_uv;

void main()
{
	out_position = in_position;
	out_normal = in_normal;
	out_uv = in_uv;
	gl_Position = projection * view * model * vec4(in_position, 1.0);
}

#endif

#ifdef FRAGMENT

layout (binding = 0) uniform sampler2D albedo;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

layout (location = 0) out vec4 colour;

void main()
{
	colour = texture(albedo, uv);
}

#endif
