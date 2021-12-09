#include <algorithm>
#include <assert.h>
#include "debug.h"

#include "PlayerV2.h"
#include "Game.h"

#include "Gate.h"
#include "Sound.h"

PlayerV2::PlayerV2(float x, float y) : Entity()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_BIG_SOPHIA));
	untouchable = 0;
	SetState(SOPHIA_BIG_STATE_IDLE);
	tag = PLAYERV2;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	gunDam = 5;
	health = MAX_HEALTH;
	isImmortaling = false;
}
PlayerV2* PlayerV2::instance = NULL;
PlayerV2* PlayerV2::GetInstance()
{
	if (instance == NULL)
		instance = new PlayerV2();
	return instance;
}

void PlayerV2::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	//DebugOut(L"x: %f y: %f \n", x, y);
	if (isDoneDeath)
		return;
	if (health <= 0)
	{
		isDeath = true;
		vx = 0;
		vy = 0;
	}

#pragma region Timer
	if (isImmortaling && immortalTimer->IsTimeUp())
	{
		isImmortaling = false;
		immortalTimer->Reset();
	}
	if (isAutoRun)
	{
		if (directionAutoRun == 1 && abs(x - oldX) <= 82)
		{
			if (direction != 0)
			{
				vx = 0.05*direction;
				vy = 0;
			}
		}
		else if (directionAutoRun == 2 && abs(y - oldY) <= 125)
		{
			if (directionY != 0)
			{
				vy = 0.07 * directionY;
				vx = 0;
			}
		}
		else
		{
			isAutoRun = false;
		}
		Entity::Update(dt);
		x += dx;
		y += dy;
		return;
	}
#pragma endregion
	Entity::Update(dt);
#pragma region Xử lý va chạm
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	// turn off collision when die 
	if (state != SOPHIA_BIG_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

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

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj->GetType() == EntityType::BRICK)
			{
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
				if (e->ny != 0)
				{

					if (e->ny != 0)
					{
						vy = 0;
						//if (ny < 0)
							//isJumping = false;
					}

					if (e->nx != 0)
						vx = 0;
				}
			}
			/*if (e->obj->GetType() == EntityType::ENEMY)
				SetInjured(1);*/
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
}

void PlayerV2::SetInjured(int dame)
{
	if (isImmortaling)
		return;
	sound->Play(GSOUND::S_HEALTH, false);
	if (health >1)
		health -= dame;
	if (gunDam > 1)
		gunDam -= dame;

	StartUntouchable();
	immortalTimer->Start();
	isImmortaling = true;
}

void PlayerV2::Render()
{
	if (isDoneDeath)
		return;
	//RenderBoundingBox();

#pragma region Khai báo biến
	int ani = -1;
	int alpha = 255;
	int current_frame; //luu frame khi dang di chuyen ma dung lai
	if (isImmortaling) alpha = 128;
#pragma endregion

	if (isDeath)
	{
		ani = SOPHIA_ANI_BIG_DIE;
		animationSet->at(ani)->Render(direction, x, y, alpha);
		if (animationSet->at(ani)->GetFrame() > LAST_FRAME_DIE)
			isDoneDeath = true;
	}
	else
	{
		if (vx == 0 && vy == 0)
		{
			if (direction > 0)
				ani = SOPHIA_ANI_BIG_IDLE_RIGHT;
			else if (direction < 0)
				ani = SOPHIA_ANI_BIG_IDLE_LEFT;
			else if (directionY > 0)
				ani = SOPHIA_ANI_BIG_IDLE_BOT;
			else if (directionY < 0)
				ani = SOPHIA_ANI_BIG_IDLE_TOP;
		}
		else
		{
			if (vx > 0)
				ani = SOPHIA_ANI_BIG_WALKING_RIGHT;
			else if (vx < 0)
				ani = SOPHIA_ANI_BIG_WALKING_LEFT;
			else if (vy > 0)
				ani = SOPHIA_ANI_BIG_WALKING_BOT;
			else if (vy < 0)
				ani = SOPHIA_ANI_BIG_WALKING_TOP;
		}
		animationSet->at(ani)->OldRender(x, y, alpha);
		//RenderBoundingBox();
	}
}

void PlayerV2::SetState(int state)
{
	Entity::SetState(state);

	switch (state)
	{
	case SOPHIA_BIG_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case SOPHIA_BIG_STATE_WALKING_RIGHT:
		direction = 1;
		directionY = 0;
		vx = SOPHIA_BIG_WALKING_SPEED;
		break;
	case SOPHIA_BIG_STATE_WALKING_LEFT:
		direction = -1;
		directionY = 0;
		vx = -SOPHIA_BIG_WALKING_SPEED;
		break;
	case SOPHIA_BIG_STATE_WALKING_TOP:
		direction = 0;
		directionY = -1;
		vy = -SOPHIA_BIG_WALKING_SPEED;
		break;
	case SOPHIA_BIG_STATE_WALKING_BOT:
		direction = 0;
		directionY = 1;
		vy = SOPHIA_BIG_WALKING_SPEED;
		break;
	case SOPHIA_BIG_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	}
}
void PlayerV2::AutoRun(int direct)
{
	if (!isAutoRun)
	{
		isAutoRun = true;
		oldX = x;
		oldY = y;
		directionAutoRun = direct;
	}
}
void PlayerV2::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!isDoneDeath)
	{
		left = x;
		top = y;
		right = x + SOPHIA_BIG_BBOX_WIDTH;
		bottom = y + SOPHIA_BIG_BBOX_HEIGHT;
	}
}

void PlayerV2::Reset()
{
	SetState(SOPHIA_BIG_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}
