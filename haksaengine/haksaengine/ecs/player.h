#pragma once

#include "ecs/component.h"

enum PlayerColour
{
	RED = 0,
	BLUE = 1,
	GREEN = 2,
	DEFAULT = 999,
	NONE = 999
};

struct Player : public Component<Player>
{
	PlayerColour colour;
};
