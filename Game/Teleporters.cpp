#include "Teleporters.h"

void Teleporters::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!isDeath)
	{
		left = x;
		top = y;
		right = x + TELEPORTERS_BBOX_WIDTH;
		bottom = y + TELEPORTERS_BBOX_HEIGHT;
	}
	else
	{
		left = 0;
		top =0;
		right = 0;
		bottom = 0;
	}
}

Teleporters::Teleporters(float x, float y, LPGAMEENTITY t)
{
	enemyType = TELEPORTERS;
	//tag = TELEPORTERSS;
	this->x = x;
	this->y = y;
	this->target = t;
	health = 3;
	isActive = false;
	isDamaged = false;
	isDeath = 0;
	TimeFreeze->Start();
	isFreeze = 1;
}

void Teleporters::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
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
	if (state != TELEPORTERS_STATE_DIE)
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
#pragma endregion
}

void Teleporters::Render()
{
	if (isDeath)
		return;
	int ani;
	if (health <= 0)
	{
		ani = TELEPORTERS_ANI_DIE;
		animationSet->at(ani)->Render(direction, x+7, y+8);
		if (animationSet->at(ani)->GetFrame() == 2)
			SetState(TELEPORTERS_STATE_DIE);
	}
	else
	{
		if (vx > 0)
			direction = 1;
		else
			direction = -1;
		if (!isFreeze)
		{
			ani = TELEPORTERS_ANI_TELE;
		}
		else
		{
			ani = TELEPORTERS_ANI_FREEZE;
		}
		
		animationSet->at(ani)->Render(direction, x, y);
	}
	//RenderBoundingBox();
	for (int i = 0; i < bullet.size(); i++)
	{
		bullet.at(i)->Render();
	}
}



void Teleporters::SetState(int state)
{
	Entity::SetState(state);

	switch (state)
	{
	case TELEPORTERS_STATE_DIE:
		vx = 0;
		vy = 0;
		isDeath = 1;
		break;

	case TELEPORTERS_STATE_TELE:
	{
		RandomPosition();
		break;
	}
	case TELEPORTERS_STATE_FREEZE:
	{
		vx = 0;
		vy = 0;
		break;
	}


	}
}

void Teleporters::Activation()
{
	if (!isActive)
	{
		vx = 0;
	}
	else
	{
		if (!TimeFreeze->IsTimeUp() && !isFreeze)
		{
			if (TimeDelay->IsTimeUp())
			{
				SetState(TELEPORTERS_STATE_TELE);
				TimeDelay->Reset();
				TimeDelay->Start();
				if (TimeDelayShot->IsTimeUp())
				{
					Attack();
					TimeDelayShot->Reset();
					TimeDelayShot->Start();

				}
			}
		}
		else if (!TimeFreeze->IsTimeUp() && isFreeze)
		{
			SetState(TELEPORTERS_STATE_FREEZE);
		}
		else if (TimeFreeze->IsTimeUp() && !isFreeze)
		{
			isFreeze = 1;
			TimeFreeze->Reset();
			TimeFreeze->Start();

		}
		else if (TimeFreeze->IsTimeUp() && isFreeze)
		{
			isFreeze = 0;
			TimeFreeze->Reset();
			TimeFreeze->Start();
		}
	}
	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= TELEPORTERS_SITEACTIVE_PLAYER)
	{
		isActive = true;
	}
}

void Teleporters::RandomPosition()
{
	int _x, _y;
	Camera::GetInstance()->GetCenter(_x, _y);

	random_device rm;
	mt19937 t(rm());

	uniform_real_distribution<float>	randi(_x - SCREEN_WIDTH/3.3, _x + SCREEN_WIDTH/3.9); // Phía giữa màn hình
	uniform_real_distribution<float>	randj(_y - SCREEN_HEIGHT/2.5, _y + SCREEN_HEIGHT/11);  // Phía giữa mh
	x = randi(t);
	y = randj(t);

}

void Teleporters::Attack()
{
	RECT PlayerV2 = target->GetBBox();
	bullet.push_back(new BulletEyeballs(x, y, x + TELEPORTERS_BBOX_WIDTH, y + TELEPORTERS_BBOX_HEIGHT, PlayerV2.left, PlayerV2.top, PlayerV2.right, PlayerV2.bottom));
	isDamaged = 1;
}
