
#extension GL_ARB_bindless_texture : require

layout (binding = 0) uniform CameraBlock
{
	mat4 view;
	mat4 projection;
	vec3 position;
} camera;

#ifdef VERTEX

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;
layout (location = 3) in uint in_texid;

layout (location = 0) out vec3 out_position;
layout (location = 1) out vec3 out_normal;
layout (location = 2) out vec2 out_uv;
layout (location = 3) out flat uint out_texid;

void main()
{
	out_position = in_position;
	out_normal = in_normal;
	out_uv = in_uv;

	gl_Position = camera.projection * camera.view * vec4(in_position.xyz, 1.0);
}

#endif

#ifdef FRAGMENT

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in flat uint tex_id;

layout (location = 0) out vec4 colour;

void main()
{	
	colour = vec4(1.0, 1.0, 1.0, 1.0);
}

#endif