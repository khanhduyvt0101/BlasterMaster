#include "Gate.h"

Gate::Gate(float x, float y, int switchId, float simonx, float simony, int simonState, bool rsCam, int type, float camX, float camY)
{
	//this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_GATE));
	tag = EntityType::GATE;
	this->x = x;
	this->y = y;
	gateSwitchSceneId = switchId;
	newPlayerx = simonx;
	newPlayery = simony;
	newPlayerState = simonState;
	directionCam = rsCam;
	typePlayer = type;
	camPosX = camX;
	camPosY = camY;
}

Gate::~Gate() {}

void Gate::Render()
{
	//RenderBoundingBox();

	//if (gateSwitchSceneId == 2 || gateSwitchSceneId == 4)	//Gate tai stage 1 3 khong ve~
	//	return;

	//animationSet->at(0)->Render(-direction, x, y);
}

void Gate::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + GATE_BBOX_WIDTH;
	b = y + GATE_BBOX_HEIGHT;
}
