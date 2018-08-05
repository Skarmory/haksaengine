
#extension GL_ARB_bindless_texture : require

#define TILE_SIZE 64
#define PALETTE_WIDTH (4 * TILE_SIZE)
#define PALETTE_HEIGHT 1
#define TILE_U_INTERVAL (float(TILE_SIZE) / float(PALETTE_WIDTH))

struct TextureData
{
	uint textures[4];
};

layout (binding = 0) uniform CameraBlock
{
	mat4 view;
	mat4 projection;
	vec3 position;
} camera;

layout (binding = 4) buffer TerrainDataBlock
{
	sampler2DArray tileset;
	TextureData data[];
} terrain_data;

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
	out_uv = vec2(in_uv.r, in_uv.g);
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

vec4 get_colour_from_tileset(uint tile_id, vec2 texcoord)
{	
	vec3 tileset_uv = vec3(texcoord, tile_id);
	
	return texture(terrain_data.tileset, tileset_uv);
}

void main()
{
	uint tile = face / 2;
	vec4 col1 = get_colour_from_tileset(terrain_data.data[tile].textures[0], uv);
	vec4 col2 = get_colour_from_tileset(terrain_data.data[tile].textures[1], uv);
	vec4 col3 = get_colour_from_tileset(terrain_data.data[tile].textures[2], uv);
	vec4 col4 = get_colour_from_tileset(terrain_data.data[tile].textures[3], uv);
	
	col1.a = max(1.0 - length(uv - vec2(0.0, 1.0)), 0.0);
	col2.a = max(1.0 - length(uv - vec2(0.0, 0.0)), 0.0);
	col3.a = max(1.0 - length(uv - vec2(1.0, 1.0)), 0.0);
	col4.a = max(1.0 - length(uv - vec2(1.0, 0.0)), 0.0);
	
	col1.rgb *= col1.a;
	col2.rgb *= col2.a;
	col3.rgb *= col3.a;
	col4.rgb *= col4.a;
	
	colour = vec4(col1.rgb + col2.rgb + col3.rgb + col4.rgb, 1.0);
}

#endif