#include "GunUp.h"

GunUp::GunUp(float x, float y)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_ITEM_GUNUP));
	tag = EntityType::GUNUP;

	this->x = x;
	this->y = y;

	displayTimer = new Timer(GUN_DISPLAY_DURATION);
	displayTimer->Start();
	delayLimit = GUN_DELAY;
}

GunUp::~GunUp() {}