#include "Cannons.h"

void Cannons::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!isDeath)
	{
		left = x;
		top = y;
		right = x + CANNONS_BBOX_WIDTH;
		bottom = y + CANNONS_BBOX_HEIGHT;
	}

}


Cannons::Cannons(float x, float y, LPGAMEENTITY t)
{
	enemyType = CANNONS;
	//tag = TELEPORTERSS;
	this->x = x;
	this->y = y;
	this->target = t;
	health = CANNONS_MAXHEALTH;
	isActive = false;
	isDamaged = false;
	isDeath = 0;
	direct = 1;
	SetState(CANNONS_STATE_NORMAL);
	TimeState->Start();
}

void Cannons::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	if (health <= 0)
		return;
	Entity::Update(dt);

	Activation();

#pragma region Xử lý bullet

	for (int i = 0; i < bullet.size(); i++)
	{
		if (target->IsCollidingObject(bullet.at(i)))
		{
			CheckBulletEnemy = 1;
		}
		bullet.at(i)->Update(dt, coObjects);
		if (bullet.at(i)->IsFinish())
			bullet.erase(bullet.begin() + i);
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
	if (state != CANNONS_STATE_DIE)
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
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void Cannons::Render()
{
	if (isDeath)
		return;
	int ani;
	if (health <= 0)
	{
		ani = CANNONS_ANI_DIE;
		animationSet->at(ani)->Render(direction, x+7, y+7);
		if (animationSet->at(ani)->GetFrame() == 2)
			SetState(CANNONS_STATE_DIE);
	}
	else if(isAttack)
	{
		if (direct==1)
			ani = CANNONS_ANI_ATTACK_HORIZONTAL;
		else
			ani = CANNONS_ANI_ATTACK_VERTICAL;
		animationSet->at(ani)->Render(direction, x, y);
	}
	else if (!isAttack)
	{
		ani = CANNONS_ANI_NORMAL;
		animationSet->at(ani)->Render(direction, x, y);
	}
	//RenderBoundingBox();
	for (int i = 0; i < bullet.size(); i++)
	{
		bullet.at(i)->Render();
	}
}

void Cannons::SetState(int state)
{
	Entity::SetState(state);

	switch (state)
	{
	case CANNONS_STATE_DIE:
	{
		vx = 0;
		vy = 0;
		isDeath = 1;
		break;
	}
	case CANNONS_STATE_NORMAL:
	{
		isAttack = 0;
		break;
	}
	case CANNONS_STATE_ATTACK_HORIZONTAL:
	{
		
		direct = 1;
		if (!isDamaged)
		{
			Attack();
			isDamaged = 0;
		}
		break;
	}
	case CANNONS_STATE_ATTACK_VERTICAL:
	{
		direct = -1;
		if (!isDamaged)
		{
			Attack();
			isDamaged = 0;
		}
		break;
	}

	}
}

void Cannons::Activation()
{
	if (!isActive)
	{
		vx = 0;
	}
	else
	{
		if (TimeState->IsTimeUp() && isAttack)
		{
			if (direct == 1)
				SetState(CANNONS_STATE_ATTACK_HORIZONTAL);
			else
				SetState(CANNONS_STATE_ATTACK_VERTICAL);
			TimeState->Reset();
			TimeState->Start();
			isAttack = 0;
			direct = -direct;
		}
		else if (TimeState->IsTimeUp() && !isAttack)
		{
			SetState(CANNONS_STATE_NORMAL);
			TimeState->Reset();
			TimeState->Start();
			isAttack = 1;
		}
	}
	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= CANNONS_SITEACTIVE_PLAYER)
	{
		isActive = true;
	}
}

void Cannons::Attack()
{
	bullet.push_back(new BulletCannons(x, y, x + CANNONS_BBOX_WIDTH-8, y + CANNONS_BBOX_HEIGHT-8, direct, 1));
	bullet.push_back(new BulletCannons(x, y, x + CANNONS_BBOX_WIDTH-8, y + CANNONS_BBOX_HEIGHT-8, direct, -1));
	isDamaged = 1;

}
