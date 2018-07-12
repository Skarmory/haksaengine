

#extension GL_ARB_bindless_texture : require

#define BONES_MAX 40

layout (binding = 0) uniform CameraBlock
{
	mat4 view;
	mat4 projection;
} camera;

layout (binding = 1) uniform PerDrawBlock
{
	mat4 model;
	mat4 bones[BONES_MAX];
	float alpha;
	uint player_colour;
	sampler2D diffuse;
} per_draw;

#ifdef VERTEX

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;
layout (location = 3) in ivec4 boneids;
layout (location = 4) in vec4 boneweights;

layout (location = 0) out vec3 out_position;
layout (location = 1) out vec3 out_normal;
layout (location = 2) out vec2 out_uv;

void main()
{
	mat4 bone_transform = (per_draw.bones[boneids[0]] * boneweights[0]);
	bone_transform     += (per_draw.bones[boneids[1]] * boneweights[1]);
	bone_transform     += (per_draw.bones[boneids[2]] * boneweights[2]);
	bone_transform     += (per_draw.bones[boneids[3]] * boneweights[3]);

	vec4 pos = bone_transform * vec4(in_position, 1.0);

	out_normal = in_normal;
	out_uv = in_uv;
	
	gl_Position = camera.projection * camera.view * per_draw.model * vec4(pos.xyz, 1.0);
}

#endif

#ifdef FRAGMENT

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

layout (location = 0) out vec4 colour;

const vec3 PLAYER_COLOURS[3] = vec3[3](
	vec3(1.0, 0.1, 0.1),
	vec3(0.0, 1.0, 0.0),
	vec3(0.0, 0.0, 1.0)
);

void main()
{
	vec4 src = texture(per_draw.diffuse, uv);
	
	vec4 dst = vec4(PLAYER_COLOURS[per_draw.player_colour].xyz, 1.0);
	
	vec3 final = (src.a * src.rgb) + ((1.0 - src.a) * dst.rgb);
	
	colour = vec4(final.rgb, per_draw.alpha);
}

#endif
