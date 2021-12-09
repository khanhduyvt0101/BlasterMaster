#include "PowerUp.h"

PowerUp::PowerUp(float x, float y)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_ITEM_POWERUP));
	tag = EntityType::POWERUP;

	this->x = x;
	this->y = y;

	displayTimer = new Timer(POWERUP_DISPLAY_DURATION);
	displayTimer->Start();
	delayLimit = POWERUP_DELAY;
}

PowerUp::~PowerUp() {}