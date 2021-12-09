#pragma once
#include "Item.h"

#define GUN_HP_RESTORE			1

#define GUN_DISPLAY_DURATION	9500
#define GUN_DELAY				600

class GunUp : public Item
{
public:
	GunUp(float x, float y);
	~GunUp();
};

