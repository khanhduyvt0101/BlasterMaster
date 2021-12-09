#include "BulletEnemy.h"

BulletEnemy::BulletEnemy()
{
	isFinish = true;
	lifeTime = 0;
	tag = BULLETNENEMY;
	BULLETENEMY = Null;
	CheckBulletEnemy = 0;

}

BulletEnemy::~BulletEnemy()
{
}

bool BulletEnemy::IsFinish()
{
	return isFinish;
}

void BulletEnemy::SetFinish()
{
	isFinish = 1;
}

EntityType BulletEnemy::GetType()
{
	return EntityType();
}

bool BulletEnemy::IsContain(RECT rect1, RECT rect2)
{
	if (rect1.left > rect2.right || rect1.right < rect2.left || rect1.top > rect2.bottom || rect1.bottom < rect2.top)
	{
		return false;
	}
	return true;
}

void BulletEnemy::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

bool BulletEnemy::Attack(float _x, float _y, int nx)
{
	return false;
}

D3DXVECTOR2 BulletEnemy::RadialMovement(D3DXVECTOR2 focus, D3DXVECTOR2 pos, float speed)
{
	

	float dist = GetDistance(pos, focus);
	if (dist < speed)
		return focus - pos;
	else
		return (focus - pos) * speed / dist;
}

float BulletEnemy::GetDistance(D3DXVECTOR2 pos, D3DXVECTOR2 target)
{
	return (float)sqrt(pow(pos.x - target.x, 2) + pow(pos.y - target.y, 2));
}

D3DXVECTOR2 BulletEnemy::CreatePosFollowTarget(D3DXVECTOR2 focus, D3DXVECTOR2 pos)
{
	D3DXVECTOR2 realtarget;
	D3DXVECTOR2 vectordirection = focus - pos;
	realtarget.x = focus.x + vectordirection.x * 1000;
	realtarget.y = focus.y + vectordirection.y * 1000;
	return realtarget;
}






