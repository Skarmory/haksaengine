#include "texture_loader.h"

TextureLoader::TextureLoader(const std::string& directory_path) : Loader(directory_path, ".png")
{
}

Texture* TextureLoader::load(const std::string& name)
{
	std::string path = get_path(name);

	std::vector<unsigned char> pixel_data;
	unsigned int width, height;

	// Just use lodepng to load the actual data in
	lodepng::decode(pixel_data, width, height, path);

	Texture* texture = new Texture(pixel_data.data(), pixel_data.size() * sizeof(unsigned char), width, height, FilterMode::Linear, TextureFormat::UnsignedByte, true);

	texture->initialise();

	return texture;
}