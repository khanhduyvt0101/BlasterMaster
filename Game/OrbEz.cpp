#include "OrbEz.h"

void OrbEz::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isDeath == false)
	{
		left = x;
		top = y;
		right = x + ORBEZ_BBOX_WIDTH;
		bottom = y + ORBEZ_BBOX_HEIGHT;
	}
}

OrbEz::OrbEz(float x, float y, LPGAMEENTITY t)
{
	enemyType = ORBEZ;
	this->x = x;
	this->y = y;
	this->target = t;
	isDeath = 0;
	health = ORBEZ_MAXHEALTH;
	isActive = false;
	isDamaged = 0;
	SetState(ORBEZ_STATE_FLY_RIGHT);
	direction = 1;
	TimerRotation->Start();
	isRotation = 0;
}

void OrbEz::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	if (health <= 0)
		return;
	Entity::Update(dt);

	
	

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
	if (state != ORBEZ_STATE_DIE)
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

		if (nx==1)
		{
			vx = -vx;
			direction = -direction;
			SetState(ORBEZ_STATE_COLLISION_FLY_RIGHT_BEHIND);
		}
		else if (nx == -1)
		{
			vx = -vx;
			direction = -direction;
			SetState(ORBEZ_STATE_COLLISION_FLY_RIGHT_INFRONTOF);
		}
		
	}
#pragma endregion
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	
#pragma region Xử lý Active
	Activation();

#pragma endregion
}

void OrbEz::Render()
{
	if (isDeath)
		return;
	
	if (health <= 0)
	{
		ani = ORBEZ_ANI_DIE;
		animationSet->at(ani)->OldRender(x+3, y+3);
		if (animationSet->at(ani)->GetFrame() == 3)
		{
			SetState(ORBEZ_STATE_DIE);
			isDeath = 1;
		}
	}
	else if (ani == ORBEZ_ANI_COLLISION_FLY_RIGHT_BEHIND)
	{
		
		animationSet->at(ani)->Render(direction, x, y);
		if (animationSet->at(ani)->GetFrame() == 3)
		{
			ani = ORBEZ_ANI_FLY_RIGHT;
			SetState(ORBEZ_ANI_FLY_RIGHT);
		}
	}
	else if (ani == ORBEZ_ANI_COLLISION_FLY_RIGHT_INFRONTOF)
	{
		animationSet->at(ani)->Render(direction, x, y);
		if (animationSet->at(ani)->GetFrame() == 4)
		{
			ani = ORBEZ_ANI_FLY_RIGHT;
			SetState(ORBEZ_ANI_FLY_RIGHT);
		}
			
	}
	else if (ani == ORBEZ_ANI_FLY_ROTATE_RIGHT360)
	{

		ani = ORBEZ_ANI_FLY_ROTATE_RIGHT360;
		animationSet->at(ani)->Render(direction, x, y);
		if (animationSet->at(ani)->GetFrame() == 7)
		{
			ani = ORBEZ_ANI_FLY_RIGHT;
			isRotation = 0;
		}
	}
	else
	{
		if (vx < 0)
			direction = -1;
		else direction = 1;
		ani = ORBEZ_ANI_FLY_RIGHT;
		animationSet->at(ani)->Render(direction,x, y);
	}
	//RenderBoundingBox();
}

void OrbEz::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case ORBEZ_STATE_DIE:
		health = 0;
		break;
	case ORBEZ_STATE_FLY_RIGHT:
		if (direction > 0)
		{
			vx = ORBEZ_WALKING_SPEED;
			vy = 0;
		}
		else
		{
			vx = -ORBEZ_WALKING_SPEED;
			vy = 0;
		}

		//DebugOut(L"\nisRotation:  %f", isRotation);
		break;
	case ORBEZ_STATE_FLY_ROTATE_RIGHT360:
		if (isRotation)
		{
			random_device rm;
			mt19937 t(rm());
			uniform_int_distribution<int>	randi(0, 1);
			if (randi(t) == 0)
			{
				if (vx>0)
				{
					vx = 0.0000001;
					vy = ORBEZ_GRAVITY;
				}
				else
				{
					vx = -0.0000001;
					vy = ORBEZ_GRAVITY;
				}
				
			}
			else
			{
				if (vx > 0)
				{
					vx = 0.0000001;
					vy = -ORBEZ_GRAVITY;
				}
				else
				{
					vx = -0.0000001;
					vy = -ORBEZ_GRAVITY;
				}
			}
		}
		ani = ORBEZ_ANI_FLY_ROTATE_RIGHT360;
		break;
	case ORBEZ_STATE_COLLISION_FLY_RIGHT_INFRONTOF:
		vy = 0;
		ani = ORBEZ_ANI_COLLISION_FLY_RIGHT_INFRONTOF;
		break;
	case ORBEZ_STATE_COLLISION_FLY_RIGHT_BEHIND:
		vy = 0;
		ani = ORBEZ_ANI_COLLISION_FLY_RIGHT_BEHIND;
		break;
	}
}

void OrbEz::Activation()
{
	if (!isActive)
	{
		vx = 0;
		vy = 0;
	}
	else
	{
		if (TimerRotation->IsTimeUp())
		{

			TimerRotation->Reset();
			TimerRotation->Start();
			isRotation = 1;
			SetState(ORBEZ_STATE_FLY_ROTATE_RIGHT360);
		}
		else if (ani == ORBEZ_ANI_FLY_RIGHT && !isRotation)
		{
			SetState(ORBEZ_STATE_FLY_RIGHT);
		}
	}
	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= ORBEZ_SITEACTIVE_PLAYER && !isActive)
	{
		isActive = true;
	}
}
