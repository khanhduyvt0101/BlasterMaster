#include "BrickBreaker.h"

void BrickBreaker::Render()
{
	int ani;
	if (isDeath)
		return;
	if (health <= 0)
	{
		ani = 1;
		animationSet->at(ani)->Render(1, x+7, y);
		//DebugOut(L"dsadasdasd %d ", animationSet->at(ani)->GetFrame());
		if (animationSet->at(ani)->GetFrame() == 3)
			isDeath = true;
	}
	else
	{
		ani = 0;
		animationSet->at(ani)->Render(1, x, y);
	}
	//RenderBoundingBox();
}

void BrickBreaker::Update(DWORD dt, vector<LPGAMEENTITY>* objects)
{
}

void BrickBreaker::Activation()
{
}

void BrickBreaker::GetCenterBoundingBox(float& x, float& y)
{
}

float BrickBreaker::GetDistance(D3DXVECTOR2 pos, D3DXVECTOR2 target)
{
	return 1;
}


bool BrickBreaker::IsContain(RECT rect1, RECT rect2)
{
	return 0;
}

void BrickBreaker::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (!isDeath)
	{
		l = x;
		t = y;
		r = x + frameW;
		b = y + frameH;
	}
}

BrickBreaker::BrickBreaker()
{
	health = 1;
	tag = EntityType::ENEMY;
	this->frameH = BRICK_BBOX_HEIGHT;
	this->frameW = BRICK_BBOX_WIDTH;
}
