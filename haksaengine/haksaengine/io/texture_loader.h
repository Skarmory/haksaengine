#pragma once

#include <string>

#include "lodepng.h"

#include "loader.h"
#include "gfx/texture.h"

// Loader for texture files.
// Currently uses lodepng to load, so only handles PNG
// TODO: Handle more image file types
class TextureLoader : public Loader
{
public:
	TextureLoader(const std::string& directory_path);

	Texture* load(const std::string& name) override;
};
