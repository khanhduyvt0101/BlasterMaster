#include "BulletCannons.h"

void BulletCannons::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!_isFinish)
	{
		left = x;
		top = y;
		right = x + BULLET_BBOX_WIDTH;
		bottom = y + BULLET_BBOX_HEIGHT;
	}
}

BulletCannons::BulletCannons(float _posLeft, float _posTop, float _posRight, float _posBottom,int _direct, int _lr)
{
	tag = BULLETENEMY = BULLETCANNONS;
	direct = _direct;
	lr = _lr; //1 trái, -1 phải //1 trên, -1 dưới
	SetCenterBoundingBox(x, y, _posLeft, _posTop, _posRight, _posBottom);
	isFinish = false;
	_isFinish = 0;
	///aniBullet = CAnimations::GetInstance()->Get(BULLET_CANNONS_ANI_FLY);

}

void BulletCannons::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	Entity::Update(dt);

	if (!_isFinish)
	{
		if (direct == 1)
		{
			vy = 0;
			if (lr<0)
				vx = BULLET_SPEED;
			else 
				vx = -BULLET_SPEED;
		}
		else
		{
			vx = 0;
			if (lr < 0)
				vy = BULLET_SPEED;
			else
				vy = -BULLET_SPEED;
		}
		
	}
	else
	{
		vx = 0;
		vy = 0;
	}


#pragma region Xử lý tiền va chạm
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEENTITY> collision;

	coEvents.clear();
	collision.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
		if (coObjects->at(i)->GetType() == EntityType::BRICK || coObjects->at(i)->GetType() == EntityType::PLAYER)
			collision.push_back(coObjects->at(i));

	// turn off collision when die 
	CalcPotentialCollisions(&collision, coEvents);

#pragma endregion


#pragma region Xử lý logic khi va chạm
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (!nx || !ny)
			_isFinish = 1;

	}
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void BulletCannons::Render()
{
	if (_isFinish)
		return;
	else
	{
		aniBullet = CAnimations::GetInstance()->Get(BULLET_CANNONS_ANI_FLY);
		RenderBoundingBox();
		aniBullet->OldRender(x, y);
	}
}

int BulletCannons::GetDamage()
{
	return CANNONS_BULLET_DAMAGE;
}

void BulletCannons::SetCenterBoundingBox(float& x, float& y, float _posLeft, float _posTop, float _posRight, float _posBottom)
{
	x = (_posLeft + _posRight) / 2;
	y = (_posTop + _posBottom) / 2;
}
