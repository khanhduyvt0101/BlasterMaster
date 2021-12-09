#include "BulletEyeballs.h"

void BulletEyeballs::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!_isFinish)
	{
		left = x;
		top = y;
		right = x + BULLET_BBOX_WIDTH;
		bottom = y + BULLET_BBOX_HEIGHT;
	}
}

BulletEyeballs::BulletEyeballs(float _x, float _y, float _posRight, float _posBottom, float _postargetLeft, float _postargetTop, float _postargetRight, float _postargetBottom)
{
	tag = BULLETENEMY = BULLETEYEBALLS;

	isFinish = false;
	_isFinish = 0;
	aniBullet = CAnimations::GetInstance()->Get(BULLET_EYEBALLS_ANI_FLY);
	RenderSpeedFollowTarget(_x, _y, _posRight, _posBottom, _postargetLeft, _postargetRight, _postargetTop, _postargetBottom, EYEBALLS_BULLET_SPEED);
}

void BulletEyeballs::Render()
{
	if (_isFinish)
		return;
	else
	{
		aniBullet = CAnimations::GetInstance()->Get(BULLET_EYEBALLS_ANI_FLY);
		RenderBoundingBox();
		aniBullet->OldRender(x, y);
	}
}

void BulletEyeballs::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
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

void BulletEyeballs::RenderSpeedFollowTarget(float _posLeft, float _posTop, float _posRight, float _posBottom, float _postargetLeft, float _postargetRight, float _postargetTop, float _postargetBottom, float _BULLET_SPEED)
{
	SetCenterBoundingBox(x, y, _posLeft, _posTop, _posRight, _posBottom);
	posBullet = D3DXVECTOR2(x, y);
	focus = CreatePosFollowTarget(D3DXVECTOR2((_postargetLeft + _postargetRight) / 2, (_postargetTop + _postargetBottom) / 2), posBullet);
	posBullet = RadialMovement(focus, posBullet, _BULLET_SPEED);
	RenderVx = posBullet.x / 14;
	RenderVy = posBullet.y / 14;
}



int BulletEyeballs::GetDamage()
{
	return EYEBALLS_BULLET_DAMAGE;
}

bool BulletEyeballs::IsStart()
{
	return isStart;
}

void BulletEyeballs::Start(float _x, float _y)
{
	isStart = 1;
	x = _x;
	y = _y;
}

void BulletEyeballs::SetCenterBoundingBox(float& x, float& y, float _posLeft, float _posTop, float _posRight, float _posBottom)
{
	x = (_posLeft + _posRight) / 2;
	y = (_posTop + _posBottom) / 2;
}
