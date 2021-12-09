#include "Stair.h"


Stair::Stair(float x, float y)
{
	//this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_GATE));
	tag = EntityType::STAIRS;
	this->x = x;
	this->y = y;
}
	Stair::~Stair() {}

void Stair::Render()
{
	//RenderBoundingBox();
}

void Stair::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + STAIR_BBOX_WIDTH;
	b = y + STAIR_BBOX_HEIGHT;

}
