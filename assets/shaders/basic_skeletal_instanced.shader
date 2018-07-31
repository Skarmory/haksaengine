

#extension GL_ARB_bindless_texture : require

#pre_process_define MAX_INSTANCES

layout (binding = 0) uniform CameraBlock
{
	mat4 view;
	mat4 projection;
	vec3 position;
} camera;

layout (binding = 1) uniform PerDrawBlock
{
	sampler2D diffuse;
	sampler2D animation_data;
} per_draw;

layout (binding = 2) uniform SceneBlock
{
	vec4 sun_direction;
	vec3 sun_colour;
} scene;

struct PerInstanceData
{
	mat4 model;
	uint animation_offset;
	uint frame_offset;
	uint player_colour;
	float alpha;
};

layout (binding = 3) uniform PerInstanceBlock
{
	PerInstanceData data[MAX_INSTANCES];
} per_instance;

#ifdef VERTEX

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;
layout (location = 3) in ivec4 boneids;
layout (location = 4) in vec4 boneweights;

layout (location = 0) out vec3 out_position;
layout (location = 1) out vec3 out_normal;
layout (location = 2) out vec2 out_uv;
layout (location = 3) out flat uint out_instance;

mat4 load_bone_transform(uint bone_id, uint animation_offset, uint frame_offset)
{
	uint base_index = animation_offset + frame_offset;
	
	base_index += (4 * bone_id);
	
	ivec2 dims = textureSize(per_draw.animation_data, 0);
	
	uint base_u = base_index % dims.x;
	uint base_v = base_index / dims.y;
	
	vec4 col1 = texelFetch(per_draw.animation_data, ivec2(base_u, base_v), 0);
	vec4 col2 = texelFetch(per_draw.animation_data, ivec2(base_u+1, base_v), 0);
	vec4 col3 = texelFetch(per_draw.animation_data, ivec2(base_u+2, base_v), 0);
	vec4 col4 = texelFetch(per_draw.animation_data, ivec2(base_u+3, base_v), 0);
	
	return mat4(col1, col2, col3, col4);
}

void main()
{
	mat4 bone_transform = (load_bone_transform(boneids[0], per_instance.data[gl_InstanceID].animation_offset, per_instance.data[gl_InstanceID].frame_offset) * boneweights[0]);
	
	if(boneweights[1] > 0.0)
		bone_transform += (load_bone_transform(boneids[1], per_instance.data[gl_InstanceID].animation_offset, per_instance.data[gl_InstanceID].frame_offset) * boneweights[1]);
	
	if(boneweights[2] > 0.0)
		bone_transform += (load_bone_transform(boneids[2], per_instance.data[gl_InstanceID].animation_offset, per_instance.data[gl_InstanceID].frame_offset) * boneweights[2]);
	
	if(boneweights[3] > 0.0)
		bone_transform += (load_bone_transform(boneids[3], per_instance.data[gl_InstanceID].animation_offset, per_instance.data[gl_InstanceID].frame_offset) * boneweights[3]);
    
	vec4 pos = bone_transform * vec4(in_position, 1.0);

	out_position = (per_instance.data[gl_InstanceID].model * pos).xyz;
	out_normal = normalize((per_instance.data[gl_InstanceID].model * (bone_transform * vec4(in_normal, 0.0))).xyz);
	out_uv = in_uv;

	gl_Position = camera.projection * camera.view * per_instance.data[gl_InstanceID].model * vec4(pos.xyz, 1.0);

	out_instance = gl_InstanceID;
}

#endif

#ifdef FRAGMENT

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in flat uint instance;

layout (location = 0) out vec4 colour;

const vec3 PLAYER_COLOURS[3] = vec3[3](
	vec3(1.0, 0.1, 0.1),
	vec3(0.1, 1.0, 0.1),
	vec3(0.1, 0.1, 1.0)
);

vec3 add_player_colour(vec4 src, uint player_id)
{
	vec4 dst = vec4(PLAYER_COLOURS[player_id].xyz, 1.0);
	
	return (src.a * src.rgb) + ((1.0 - src.a) * dst.rgb);
}

void main()
{	
	vec3 diffuse_albedo = add_player_colour(texture(per_draw.diffuse, uv), per_instance.data[instance].player_colour);
	
	vec3 to_cam = normalize(camera.position - position);

	vec3 diffuse = max(dot(normal, -scene.sun_direction.xyz), 0.0) * diffuse_albedo;
	
	colour = vec4(diffuse, per_instance.data[instance].alpha);
}

#endif
