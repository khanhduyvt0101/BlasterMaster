#include "GateImage.h"

GateImage::GateImage(int isHorizal)
{
	isHori = isHorizal;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(GATE_IMAGE_ANIMATION_SET));
	tag = EntityType::GATEIMAGE;
}

GateImage::~GateImage() {}

void GateImage::Render()
{
	if (isHori)
		animationSet->at(0)->Render(1, x, y);
	else
		animationSet->at(1)->Render(1, x, y);
}

void GateImage::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}
