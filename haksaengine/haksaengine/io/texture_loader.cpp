#include "texture_loader.h"

TextureLoader::TextureLoader(const std::string& directory_path) : Loader(directory_path, ".png")
{
}

Texture* TextureLoader::load(const std::string& name)
{
	Texture* texture = new Texture;

	std::string path = get_path(name);

	// Just use lodepng to load the actual data in
	lodepng::decode(texture->image, texture->width, texture->height, path);

	texture->initialise();

	return texture;
}