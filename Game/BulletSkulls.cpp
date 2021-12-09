#include "BulletSkulls.h"

BulletSkulls::BulletSkulls(float _x, float _y, float _vx)
{
	x = _x;
	y = _y;
	tag = BULLETENEMY = BULLETFLOATERS;
	isFinish = false;
	aniBullet = CAnimations::GetInstance()->Get(BULLET_SKULLS_ANI_FLY);
	vx = _vx;
	isBang = 0;
	TimeBang->Reset();
	TimeBang->Start();
	_isFinish = 0;
}

void BulletSkulls::Render()
{
	if (_isFinish)
	{
		aniBullet = CAnimations::GetInstance()->Get(BULLET_SKULLS_ANI_BANG);
		RenderBoundingBox();
		aniBullet->OldRender(x, y);
		if (CAnimations::GetInstance()->Get(BULLET_SKULLS_ANI_BANG)->GetFrame() == 3) 
		{
			isFinish = 1;
			return;
		}
	}
	else
	{
		aniBullet = CAnimations::GetInstance()->Get(BULLET_SKULLS_ANI_FLY);
		aniBullet->OldRender(x, y);
	}
	
	//aniBullet->OldRender(x, y);
	//RenderBoundingBox();

}

void BulletSkulls::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	vy += BULLET_GRAVYTY_VY;
	if (vx > 0)
	{
		vx -= BULLET_FRICTION_VX;
	}
	else
	{
		vx += BULLET_FRICTION_VX;
	}
	Entity::Update(dt);

	if (TimeBang->IsTimeUp())
	{
		_isFinish = 1;
	}
	/*x += dx;
	y += dy;*/
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

		if (ny != 0)
		{
			
			if (vy<0)
			{
				vy = -vy/2;
			}
			else
			{
				vy = -vy/2;
			}

		}
		if (nx!=0)
		{
			vx = -vx;
		}
	
			
	}
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void BulletSkulls::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!_isFinish)
	{
		left = x;
		top = y;
		right = x + BULLET_BBOX_WIDTH;
		bottom = y + BULLET_BBOX_HEIGHT;
	}
}

int BulletSkulls::GetDamage()
{
	return SKULLS_BULLET_DAMAGE;
}

bool BulletSkulls::IsStart()
{
	return false;
}
