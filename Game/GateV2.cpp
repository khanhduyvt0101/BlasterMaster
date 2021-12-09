#include "GateV2.h"

GateV2::GateV2(float x, float y, float newXGo, float newXBack, float newYGo, float newYBack, int directCam)
{
	//this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_GATE));
	tag = EntityType::GATEV2;
	this->x = x;
	this->y = y;
	intoXGo = newXGo;
	intoXBack = newXBack;
	intoYGo = newYGo;
	intoYBack = newYBack;
	directionCam = directCam;
}

GateV2::~GateV2() {}

void GateV2::Render()
{
	//RenderBoundingBox();

	//if (gateSwitchSceneId == 2 || gateSwitchSceneId == 4)	//Gate tai stage 1 3 khong ve~
	//	return;

	//animationSet->at(0)->Render(-direction, x, y);
}

void GateV2::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	if (directionCam == 1)
	{
		r = x + GATEV2_BBOX_WIDTH;
		b = y + GATEV2_BBOX_HEIGHT;
	}
	else
	{
		r = x + GATEV2_BBOX_HEIGHT;
		b = y + GATEV2_BBOX_NEW_HEIGHT;
	}
}
