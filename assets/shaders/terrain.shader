
#extension GL_ARB_bindless_texture : require

#define TILE_SIZE 64
#define PALETTE_WIDTH (4 * TILE_SIZE)
#define PALETTE_HEIGHT 1
#define TILE_U_INTERVAL (float(TILE_SIZE) / float(PALETTE_WIDTH))

struct TextureData
{
	uint textures[3];
};

layout (binding = 0) uniform CameraBlock
{
	mat4 view;
	mat4 projection;
	vec3 position;
} camera;

layout (binding = 4) buffer TextureDataBlock
{
	sampler2D tileset;
	TextureData data[];
} tex_data;

#ifdef VERTEX

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;
layout (location = 3) in uint in_face;

layout (location = 0) out vec3 out_position;
layout (location = 1) out vec3 out_normal;
layout (location = 2) out vec2 out_uv;
layout (location = 3) out flat uint out_face;

void main()
{
	out_position = in_position;
	out_normal = in_normal;
	out_uv = in_uv;
	out_face = in_face;

	gl_Position = camera.projection * camera.view * vec4(in_position.xyz, 1.0);
}

#endif

#ifdef FRAGMENT

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in flat uint face;

layout (location = 0) out vec4 colour;

vec4 get_colour_from_tileset(uint tile_id, vec2 uv)
{
	vec2 tileset_uv;
	tileset_uv.r = (tile_id * TILE_U_INTERVAL) + (uv.r * TILE_U_INTERVAL);
	tileset_uv.g = (uv.g * TILE_U_INTERVAL);
	
	return texture(tex_data.tileset, tileset_uv);
}

void main()
{
	vec4 col1 = get_colour_from_tileset(tex_data.data[face].textures[0], uv);
	//vec4 col2 = get_colour_from_tileset(tex_data.data[face].textures[1], uv);
	//vec4 col3 = get_colour_from_tileset(tex_data.data[face].textures[2], uv);
	
	//colour = (col1 + col2 + col2) / 3.0;
	
	colour = col1;
}

#endif