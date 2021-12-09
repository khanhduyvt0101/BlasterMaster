#include "Centipede.h"
#include "Brick.h"
Centipede::Centipede(float x, float y, LPGAMEENTITY t)
{
	SetState(CENTIPEDE_STATE_WALKING);
	enemyType= CENTIPEDE;
	this->x = x;
	this->y = y;
	nx = -1;
	isFollow = 0;
	this->target = t;
	health = CENTIPEDE_MAXHEALTH;
	isActive = false;
	alpha = 255;
}

void Centipede::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!isDeath)
	{
		left = x;
		top = y;
		right = x + CENTIPEDE_BBOX_WIDTH;
		bottom = y + CENTIPEDE_BBOX_HEIGHT;
	}
}

void Centipede::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	if (health <= 0)
		return;
	Entity::Update(dt);

#pragma region Xử lý vy khi rơi
	if (isActive)
		vy = 13*CENTIPEDE_GRAVITY * dt;
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
	if (state != CENTIPEDE_STATE_DIE)
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

		


		//Follow theo player
		if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= CENTIPEDE_SITEFOLLOW_PLAYER)// Kiểm tra bán kính xung quanh Golem xem có player không
		{
			FollowTarget(target);
		}
		else // Đụng tường hay hết brick quay lại
		{
			if (nx != 0)
			{
				this->nx = -this->nx;
				//DebugOut(L"\nnx:  %d", nx);
			}
			if (ny != 0)
			{
				vy = 0;
				for (int i = 0; i < coEventsResult.size(); i++)
				{
					LPCOLLISIONEVENT e = coEventsResult.at(i);
					if (e->ny != 0)
					{
						RECT rect = static_cast<Brick*>(e->obj)->GetBBox();
						if (x + CENTIPEDE_BBOX_WIDTH > rect.right)
						{
							this->nx = -this->nx;
							x += rect.right - (x + CENTIPEDE_BBOX_WIDTH) - nx * 0.4f;
						}
						else if (x < rect.left)
						{
							this->nx = -this->nx;
							x += rect.left - x + nx * 0.4f;
						}
						break;
					}
				}
				//DebugOut(L"\nny:  %d", ny);
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
	/*SelfDestroy();*/
#pragma region Xử lý Active
	Activation();

#pragma endregion
}

//void Centipede::SelfDestroy()
//{
//	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->Getx(), target->Gety())) <= CENTIPEDE_SITEFOLLOW_PLAYER)
//	{
//		Player* pl = dynamic_cast<Player*>(target);
//		if (!pl->IsImmortaling())
//		{
//			pl->AddHealth(-1);
//			pl->StartImmortalingTimer();
//			pl->SetImmortaling(true);
//		}
//		//SetState(DARKBAT_STATE_DIE);
//		return;
//	}
//}

//co van de
void Centipede::FollowTarget(LPGAMEENTITY target)
{
	if ((target->x - this->x) > 0)
		this->nx = 1;
	else
		this->nx = -1;
	//vx = -CENTIPEDE_WALKING_SPEED;
}

void Centipede::Render()
{
	if (isDeath)
		return;
	int ani;
	if (health <= 0)
	{
		ani = CENTIPEDE_ANI_DIE;
		animationSet->at(ani)->Render(direction, x+2, y);
		//DebugOut(L"dsadasdasd %d ", animationSet->at(ani)->GetFrame());
		if (animationSet->at(ani)->GetFrame() == 3)
			SetState(CENTIPEDE_STATE_DIE);
	}
	else
	{
		if (vx > 0)
			direction = 1;
		else
			direction = -1;

		int ani = CENTIPEDE_ANI_WALKING;
		if (state == CENTIPEDE_STATE_DIE) {
			ani = CENTIPEDE_ANI_DIE;
		}
		//DebugOut(L"[xxxxxx] direction: %s\n", direction);
		animationSet->at(ani)->Render(direction, x, y, alpha);
		//RenderBoundingBox();
	}
}

void Centipede::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case CENTIPEDE_STATE_DIE:
		//y += CENTIPEDE_BBOX_HEIGHT - CENTIPEDE_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		isDeath = 1;
		break;
	case CENTIPEDE_STATE_WALKING:
		if (nx > 0)
		{
			vx = CENTIPEDE_WALKING_SPEED;
		}
		else
		{
			vx = -CENTIPEDE_WALKING_SPEED;
		}
	}
}

void Centipede::Activation()
{
	if (!isActive)
	{
		vx = 0;
	}
	else
	{
		SetState(CENTIPEDE_STATE_WALKING);
	}

	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= CENTIPEDE_SITEACTIVE_PLAYER)
	{
		isActive = true;
	}
}
