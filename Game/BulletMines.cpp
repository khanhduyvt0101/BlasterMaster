#include "BulletMines.h"

BulletMines::BulletMines(float _x, float _y)
{
	x = _x;
	y = _y;
	tag = BULLETENEMY = BULLETFLOATERS;
	
	vy = -BULLET_JUMP_VY;
	isFinish = false;
	vx = RandomVx();
	aniBullet = CAnimations::GetInstance()->Get(BULLET_MINES_ANI_FLY);
	
}

float BulletMines::RandomVx()
{
	random_device rm;
	mt19937 t(rm());
	uniform_real_distribution<float>	randi(-0.03, 0.05);
	return randi(t);
	
	
}

void BulletMines::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	Entity::Update(dt);

	vy += BULLET_GRAVYTY_VY;
	
#pragma region Xử lý tiền va chạm
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEENTITY> collision;

	coEvents.clear();
	collision.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
		if (coObjects->at(i)->GetType() == EntityType::BRICK)
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

		if (ny != 0 || nx != 0)
		{
			_isFinish = 1;
		}
		
	}
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void BulletMines::Render()
{
	if (_isFinish)
	{
		aniBullet = CAnimations::GetInstance()->Get(BULLET_MINES_ANI_BANG);
		RenderBoundingBox();
		aniBullet->OldRender(x, y);
		if (CAnimations::GetInstance()->Get(BULLET_MINES_ANI_BANG)->GetFrame() == 3) //Luc nay no bang 3 hoai, phai cho no bang 0 tip
		{
			isFinish = 1;
			return;
		}
	}
	else
	{
		aniBullet = CAnimations::GetInstance()->Get(BULLET_MINES_ANI_FLY);
		aniBullet->OldRender(x, y);
	}

	//aniBullet->OldRender(x, y);
	RenderBoundingBox();

}

void BulletMines::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (!_isFinish)
	{
		l = x;
		t = y;
		r = x + BULLET_BBOX_WIDTH;
		b = y + BULLET_BBOX_HEIGHT;
	}
}

int BulletMines::GetDamage()
{
	return 0;
}


