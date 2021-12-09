#include "Orbs.h"

void Orbs::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isDeath == false)
	{
		left = x;
		top = y;
		right = x + ORBS_BBOX_WIDTH;
		bottom = y + ORBS_BBOX_HEIGHT;
	}
}

Orbs::Orbs(float x, float y, LPGAMEENTITY t)
{
	enemyType = ORBS;
	this->x = x;
	this->y = y;
	this->target = t;
	isDeath = 0;
	health = ORBS_MAXHEALTH;
	isActive = false;
	isDamaged = 0;
	isColliTarget = 0;
	
}

void Orbs::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	if (health <= 0)
		return;
	Entity::Update(dt);
	
#pragma region Xử lý follow target
	RECT _target = target->GetBBox();
	RenderSpeedFollowTarget(x, y, x + ORBS_BBOX_WIDTH, y + ORBS_BBOX_HEIGHT, _target.left, _target.right, _target.top, _target.bottom, ORBS_ATTACK_SPEED);
	if (isDamaged && !isColliTarget)
	{
		this->vx = RenderVx;
		if (GetDistance(D3DXVECTOR2(x, y), target->GetPosCam()) == ORBS_SITEATTACK_TOY)
		{
			this->vy = RenderVy;
		}
		else if (target->y < y)
		{
			if (vx < 0)
				this->vy = -RenderVx;
			else 
				this->vy = RenderVx;
		}
		else
			this->vy = RenderVy;
	}
	else
	{
		vx = 0;
		vy = 0;
	}
#pragma endregion

#pragma region Xử lý tiền va chạm
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEENTITY> bricks;

	coEvents.clear();
	bricks.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
		if (coObjects->at(i)->GetType() == EntityType::BRICK)
			bricks.push_back(coObjects->at(i));

	// turn off collision when die 
	if (state != ORBS_STATE_DIE)
		CalcPotentialCollisions(&bricks, coEvents);

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
		if (CheckColisionEnemy)
		{
			SetState(ORBS_STATE_DIE);
			isColliTarget = 1;
		}
	}
#pragma endregion

#pragma region Xử lý Active
	Activation();

#pragma endregion
}

void Orbs::Render()
{
	if (isDeath)
		return;
	int ani;
	if (health <=0)
	{
		ani = ORBS_ANI_DIE;
		animationSet->at(ani)->OldRender(x+3, y+3);
		if (animationSet->at(ani)->GetFrame() == 3)
		{
			SetState(ORBS_STATE_DIE);
			isDeath = 1;
		}
	}
	else
	{
		ani = ORBS_ANI_ATTACK;
		animationSet->at(ani)->OldRender(x, y);
	}
	//RenderBoundingBox();
}


void Orbs::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case ORBS_STATE_DIE:
		health = 0;
		break;
	case ORBS_STATE_ATTACK:
		isDamaged = 1;
		break;
	}
}

void Orbs::Activation()
{
	if (!isActive)
	{
		vx = 0;
		vy = 0;
		isDamaged = 0;
	}
	else
	{
		SetState(ORBS_STATE_ATTACK);
	}
	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= ORBS_SITEACTIVE_PLAYER)
	{
		isActive = true;
	}
}
