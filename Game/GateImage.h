#pragma once
#include "Entity.h"
#define GATE_IMAGE_ANIMATION_SET 180
class GateImage : public Entity
{
public:
	int isHori;
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	GateImage(int isHorizal);
	~GateImage();
};

