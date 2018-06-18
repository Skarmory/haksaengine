#pragma once

#include <string>

#include "lodepng.h"

#include "loader.h"
#include "gfx/texture.h"

class TextureLoader : public Loader
{
public:
	TextureLoader(const std::string& directory_path);

	Texture* load(const std::string& name) override;
};
