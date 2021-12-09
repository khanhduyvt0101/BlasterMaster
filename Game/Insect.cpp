#include "Insect.h"

void Insect::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!isDeath)
	{
		left = x;
		top = y;
		right = x + INSECT_BBOX_WIDTH;
		bottom = y + INSECT_BBOX_HEIGHT;
	}
}

Insect::Insect(float x, float y, LPGAMEENTITY t)
{
	
	enemyType = INSECT;
	this->x = x;
	this->y = y;
	nx = -1;
	this->target = t;
	SetState(INSECT_STATE_FLY);
	health = INSECT_MAXHEALTH;
	isActive = false;
	isFly = 1;
	isDeath = 0;
		
}

void Insect::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	if (health <= 0)
		return;
	
	Entity::Update(dt);
	//SelfDestroy();
#pragma region Xử lý vy
	if (delayTimerDowm->IsTimeUp() && isFly == 0)
	{
		isFly = 1;
		delayTimerDowm->Reset();
		delayTimerUp->Start();
		SetState(INSECT_STATE_FLY);
	}
	else if (delayTimerUp->IsTimeUp() && isFly == 1)
	{
		isFly = 0;
		delayTimerUp->Reset();
		delayTimerDowm->Start();
		SetState(INSECT_STATE_FLY);
	}
	
	//DebugOut(L"\nisFly:   %d\n", delayTimerDowm->IsTimeUp());
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
	if (state != INSECT_STATE_DIE)
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

		if (nx!=0 && ny==0)
		{
			this->nx = -this->nx;
			vx = -vx;
		}
		if (ny == 1)
		{
			SetState(INSECT_STATE_DOWN);
			isFly = -1;
			
		}
		else if (ny == -1)
		{
			SetState(INSECT_STATE_FLY);
			isFly = 0;
			delayTimerDowm->Reset();
		}
		
	}
#pragma endregion

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

#pragma region Xử lý Active
	Activation();

#pragma endregion
}

void Insect::Render()
{
	if (isDeath)
		return;
	int ani;
	if (health <= 0)
	{
		ani = INSECT_ANI_DIE;
		animationSet->at(ani)->Render(direction, x+3, y+3);
		//DebugOut(L"dsadasdasd %d ", animationSet->at(ani)->GetFrame());
		if (animationSet->at(ani)->GetFrame() == 3)
			SetState(INSECT_STATE_DIE);
	}
	else
	{
		if (vx < 0)
			direction = 1;
		else
			direction = -1;
		ani = INSECT_ANI_FLY;
		animationSet->at(ani)->Render(direction, x, y);
	}
	//RenderBoundingBox();
}



void Insect::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case INSECT_STATE_DIE:
		//y += GOLEM_BBOX_HEIGHT - GOLEM_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		isDeath = true;
		break;
	case INSECT_STATE_FLY:
		if (isFly)
		{
			if (nx > 0)
			{
				vx = INSECT_FLYLR_SPEED*1.3;
				vy = -INSECT_FLYUP_SPEED;
			}
			else
			{
				vx = -INSECT_FLYLR_SPEED*1.3;
				vy = -INSECT_FLYUP_SPEED;
			}
		
			
			
		}
		else if (!isFly)
		{
			if (nx > 0)
			{
				vx = INSECT_FLYLR_SPEED;
				vy = INSECT_FLYDOWN_SPEED;
			}
			else
			{
				vx = -INSECT_FLYLR_SPEED;
				vy = INSECT_FLYDOWN_SPEED;
			}
		
			
		}
		
		break;
	case INSECT_STATE_DOWN:
		vy += INSECT_GRAVITY;
		vx = -0.000000001;
		
		break;
	}
}

void Insect::Activation()
{
	if (!isActive)
	{
		vx = 0;
		vy = 0;
	}
	else
	{
		SetState(INSECT_STATE_FLY);
	}
	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= INSECT_SITEACTIVE_PLAYER)
	{
		isActive = true;
	}
}
