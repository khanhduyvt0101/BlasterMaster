#include "Enemy.h"

Enemy::Enemy()
{
	tag = ENEMY;
	CheckColisionEnemy = 0;

}

Enemy::~Enemy()
{
}

void Enemy::Render()
{
}

bool Enemy::IsContain(RECT rect1, RECT rect2)
{
	if (rect1.left > rect2.right || rect1.right < rect2.left || rect1.top > rect2.bottom || rect1.bottom < rect2.top)
	{
		return false;
	}
	return true;
}

float Enemy::GetDistance(D3DXVECTOR2 pos, D3DXVECTOR2 target)
{
	return (float)sqrt(pow(pos.x - target.x, 2) + pow(pos.y - target.y, 2));
}

D3DXVECTOR2 Enemy::GetPosCam()
{
	return D3DXVECTOR2(x,y);
}

void Enemy::SetCenterBoundingBox(float& x, float& y, float _posLeft, float _posTop, float _posRight, float _posBottom)
{
	x = (_posLeft + _posRight) / 2;
	y = (_posTop + _posBottom) / 2;
}

void Enemy::RenderSpeedFollowTarget(float _posLeft, float _posTop, float _posRight, float _posBottom, float _postargetLeft, float _postargetRight, float _postargetTop, float _postargetBottom, float _SPEED)
{
	//SetCenterBoundingBox(x, y, _posLeft, _posTop, _posRight, _posBottom);
	posEnemy = D3DXVECTOR2(x, y);
	focus = CreatePosFollowTarget(D3DXVECTOR2((_postargetLeft + _postargetRight) / 2, (_postargetTop + _postargetBottom) / 2), posEnemy);
	posEnemy = RadialMovement(focus, posEnemy, _SPEED);
	RenderVx = posEnemy.x/10;
	RenderVy = posEnemy.y/10;
}

D3DXVECTOR2 Enemy::CreatePosFollowTarget(D3DXVECTOR2 focus, D3DXVECTOR2 pos)
{
	D3DXVECTOR2 realtarget;
	D3DXVECTOR2 vectordirection = focus - pos;
	realtarget.x = focus.x + vectordirection.x * 1000;
	realtarget.y = focus.y + vectordirection.y * 1000;
	return realtarget;
}

D3DXVECTOR2 Enemy::RadialMovement(D3DXVECTOR2 focus, D3DXVECTOR2 pos, float speed)
{
	float dist = GetDistance(pos, focus);
	if (dist < speed)
		return focus - pos;
	else
		return (focus - pos) * speed / dist;
}


