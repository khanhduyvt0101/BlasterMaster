#include "BulletBoss.h"

BulletBoss::BulletBoss(float _x, float _y, float _posRight, float _posBottom, float _postargetLeft, float _postargetTop, float _postargetRight, float _postargetBottom)
{
	tag = BULLETENEMY = BULLETFLOATERS;

	isFinish = false;

	_isFinish = 0;
	aniBullet = CAnimations::GetInstance()->Get(BULLET_BOSS_ANI_FLY);
	RenderSpeedFollowTarget(_x, _y, _posRight, _posBottom, _postargetLeft, _postargetRight, _postargetTop, _postargetBottom, BOSS_BULLET_SPEED);
}

void BulletBoss::Render()
{
	if (_isFinish)
		return;
	else
	{
		aniBullet = CAnimations::GetInstance()->Get(BULLET_BOSS_ANI_FLY);
		RenderBoundingBox();
		aniBullet->OldRender(x, y);
	}
}

void BulletBoss::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	Entity::Update(dt);

	if (!_isFinish)
	{
		vx = RenderVx;
		vy = RenderVy;
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
		if (coObjects->at(i)->GetType() == EntityType::PLAYER || coObjects->at(i)->GetType() == EntityType::PLAYERV2 || coObjects->at(i)->GetType() == EntityType::BULLETV2)
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

void BulletBoss::RenderSpeedFollowTarget(float _x, float _y, float _posRight, float _posBottom, float _postargetLeft, float _postargetRight, float _postargetTop, float _postargetBottom, float _BULLET_SPEED)
{
	SetCenterBoundingBox(x, y, _x, _y, _posRight, _posBottom);
	posBullet = D3DXVECTOR2(x, y);
	focus = CreatePosFollowTarget(D3DXVECTOR2((_postargetLeft + _postargetRight) / 2, (_postargetTop + _postargetBottom) / 2), posBullet);
	posBullet = RadialMovement(focus, posBullet, _BULLET_SPEED);
	RenderVx = posBullet.x / 14;
	RenderVy = posBullet.y / 14;
}

void BulletBoss::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BULLET_BBOX_WIDTH;
	b = y + BULLET_BBOX_HEIGHT;
}

int BulletBoss::GetDamage()
{
	return BOSS_BULLET_DAMAGE;
}

bool BulletBoss::IsStart()
{
	return isStart;
}

void BulletBoss::Start(float _x, float _y)
{
}

void BulletBoss::SetCenterBoundingBox(float& x, float& y, float _posLeft, float _posTop, float _posRight, float _posBottom)
{
	x = (_posLeft + _posRight) / 2;
	y = (_posTop + _posBottom) / 2;
}
