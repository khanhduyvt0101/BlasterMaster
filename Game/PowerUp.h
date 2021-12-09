#pragma once
#include "Item.h"

#define POWER_HP_RESTORE			1

#define POWERUP_DISPLAY_DURATION	9500
#define POWERUP_DELAY				600

class PowerUp : public Item
{
public:
	PowerUp(float x, float y);
	~PowerUp();
};

