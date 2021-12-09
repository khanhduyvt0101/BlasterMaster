#pragma once
#include "Entity.h"

#define GATEV2_BBOX_WIDTH				54
#define GATEV2_BBOX_HEIGHT				20
#define GATEV2_BBOX_NEW_HEIGHT			88

class GateV2 : public Entity
{
public:
	int directionCam;
	float intoXGo, intoYGo, intoXBack, intoYBack;
	GateV2(float x, float y, float intoXGo, float intoXBack, float intoYGo, float intoYBack, int directCam);
	~GateV2();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();
};

