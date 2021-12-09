#include "Golem.h"
#include "Player.h"
#include <math.h>
Golem::Golem(float x, float y, LPGAMEENTITY t)
{
	SetState(GOLEM_STATE_WALKING);
	enemyType = GOLEM;
	this->x = x;
	this->y = y;

	nx = -1;
	isFollow = 0;
	this->target = t;
	afterFollow = 1;
	firstFollow = 1;
	health = GOLEM_MAXHEALTH;
	isActive = false;
}

void Golem::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	//DebugOut(L"vy = %f \n", vy);
	if (health <= 0)
		return;
	Entity::Update(dt);
	//SelfDestroy();
#pragma region Xử lý vy khi rơi
	vy += GOLEM_GRAVITY * dt;
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
	if (state != GOLEM_STATE_DIE)
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
		y += min_ty * dy + ny * 0.004f;
		
		

		//Follow theo player
		if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= GOLEM_SITEFOLLOW_PLAYER)// Kiểm tra bán kính xung quanh Golem xem có player không
		{
			FollowTarget(target);
		}
		else // Đụng tường hay hết brick quay lại
		{
			
			if (nx != 0)
			{
				this->nx = -this->nx;
			}
			/*if (ny > 0)
			{
				vy = 0;
				y += dy;
			}*/
			if (ny != 0)
			{
				
				vy = 0;
				for (int i = 0; i < coEventsResult.size(); i++)
				{
					LPCOLLISIONEVENT e = coEventsResult.at(i);
					if (e->ny != 0)
					{
						RECT rect = static_cast<Brick*>(e->obj)->GetBBox();
						if (x + GOLEM_BBOX_WIDTH > rect.right)
						{
							this->nx = -this->nx;
							x += rect.right - (x + GOLEM_BBOX_WIDTH) - nx * 0.4f;
						}
						else if (x < rect.left)
						{
							this->nx = -this->nx;
							x += rect.left - x + nx * 0.4f;
						}
						break;
					}
				}
			}
			
		}
	}
	
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion

#pragma region Xử lý Active
	Activation();

#pragma endregion
}

void Golem::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isDeath == false)
	{
		left = x;
		top = y;
		right = x + GOLEM_BBOX_WIDTH;
		bottom = y + GOLEM_BBOX_HEIGHT;
	}
}

int Golem::randomTimeJump()
{
	srand(time(NULL));
	int res;
	for (int i = 0; i < 5; ++i) {
		res = rand() % (GOLEM_TIME_FOLLOW_PLAYER_MAX - 500 + 1) + 500;
	}
	return res;
}


void Golem::FollowTarget(LPGAMEENTITY target)
{

	if (firstFollow)
	{
		if ((target->x - this->x) > 0)
			this->nx = 1;
		else
			this->nx = -1;
		vy = -GOLEM_JUMP_SPEED_Y;
		isFollow = true;
		readyTimer->Start();
		firstFollow = false;
	}
	else if (afterFollow)
	{
		if (delayTimer->IsTimeUp())
		{

			if ((target->x - this->x) > 0)
				this->nx = 1;
			else
				this->nx = -1;
			vy = -GOLEM_JUMP_SPEED_Y;
			isFollow = true;
			readyTimer->Start();
			afterFollow = false;
			delayTimer->Reset();
		}
	}
	else if (isFollow)
	{
		if (!readyTimer->IsTimeUp())
		{
			
			if ((target->x - this->x) > 0)
				this->nx = 1;
			else
				this->nx = -1;
			vy = -GOLEM_JUMP_SPEED_Y;
		}
		else
		{
			delayTimer->Start();
			isFollow = false;
			afterFollow = 1;
			readyTimer->Reset(randomTimeJump());
		}
	}
		
}


void Golem::Render()
{
	if (isDeath)
		return;
	int ani;
	if (health <= 0)
	{
		ani = GOLEM_ANI_DIE;
		animationSet->at(ani)->Render(direction, x+2, y+3);
		//DebugOut(L"dsadasdasd %d ", animationSet->at(ani)->GetFrame());
		if (animationSet->at(ani)->GetFrame() == 3)
			SetState(GOLEM_STATE_DIE);
	}
	else
	{
		if (vx > 0)
			direction = 1;
		else
			direction = -1;
		ani = GOLEM_ANI_WALKING;
		animationSet->at(ani)->Render(direction, x, y);
	}
	//RenderBoundingBox();
}

void Golem::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case GOLEM_STATE_DIE:
		//y += GOLEM_BBOX_HEIGHT - GOLEM_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		isDeath = true;
		break;
	case GOLEM_STATE_WALKING:
		if (nx>0)
		{
			vx = GOLEM_WALKING_SPEED;
		}
		else
		{
			vx = -GOLEM_WALKING_SPEED;
		}
	}
}

void Golem::Activation()
{
	if (!isActive)
	{
		vx = 0;
	}
	else
	{
		SetState(GOLEM_STATE_WALKING);
	}
	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= GOLEM_SITEACTIVE_PLAYER)
	{
		isActive = true;
	}
}
