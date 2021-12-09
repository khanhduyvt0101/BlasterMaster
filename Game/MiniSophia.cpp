#include <algorithm>
#include <assert.h>
#include "debug.h"

#include "MiniSophia.h"
#include "Game.h"
#include "Sound.h"

MiniSophia::MiniSophia(float x, float y) : Entity()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_MINI_SOPHIA));
	untouchable = 0;
	SetState(SOPHIA_MINI_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	backup_JumpY = 0;
	gunDam = MAX_HEALTH;
	health = MAX_HEALTH;
	isImmortaling = false;
	isCrawl = false;
	alpha = 255;
}
MiniSophia* MiniSophia::instance = NULL;
MiniSophia* MiniSophia::GetInstance()
{
	if (instance == NULL)
		instance = new MiniSophia();
	return instance;
}

void MiniSophia::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects, vector<LPGAMEENTITY>* coEnemies)
{
	if (isDoneDeath)
		return;
	if (health <= 0)
	{
		isDeath = true;
		vx = 0;
		vy = 0;
	}
	Entity::Update(dt);
#pragma region Xử lý vy
	if (!isOnStair)
		vy += SOPHIA_GRAVITY * dt;
#pragma endregion

#pragma region Timer
	if (isImmortaling && immortalTimer->IsTimeUp())
	{
		isImmortaling = false;
		immortalTimer->Reset();
	}
#pragma endregion

#pragma region Xử lý va chạm
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	// turn off collision when die 
	if (state != SOPHIA_MINI_STATE_DIE)
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
				y += min_ty * dy + ny * 0.001f;
				if (e->ny != 0)
				{

					if (e->ny != 0)
					{
						vy = 0;
						if (ny < 0)
							isJumping = false;
					}

					if (e->nx != 0)
						vx = 0;
				}
			}
		}

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
}

void MiniSophia::SetInjured(int dame)
{
	if (isImmortaling)
		return;
	sound->Play(GSOUND::S_HEALTH, false);
	health -= dame;
	gunDam -= dame;

	StartUntouchable();
	immortalTimer->Start();
	isImmortaling = true;
}

void MiniSophia::Render()
{
	if (isDoneDeath)
		return;
	//RenderBoundingBox();

#pragma region Khai báo biến
	int ani = -1;
	int current_frame; //luu frame khi dang di chuyen ma dung lai
	alpha = 255;
	if (isImmortaling) alpha = 128;
#pragma endregion

	if (isDeath)
	{
		ani = SOPHIA_ANI_MINI_DIE;
		animationSet->at(ani)->Render(direction, x , y , alpha);
		if (animationSet->at(ani)->GetFrame() > LAST_FRAME_DIE)
			isDoneDeath = true;
	}
	else
	{
		if (isJumping)
		{
			ani = SOPHIA_ANI_MINI_JUMP;
		}
		else if (isOnStair)
		{
			if (vy == 0)
				ani = SOPHIA_ANI_MINI_IDLESTAIR;
			else
				ani = SOPHIA_ANI_MINI_GOSTAIR;
		}
		else if (isCrawl)
		{
			if (vx == 0)
				ani = SOPHIA_ANI_MINI_IDLE_CRAWL_RIGHT;
			else
				ani = SOPHIA_ANI_MINI_WALKING_CRAWL_RIGHT;
		}
		else
		{
			if (vx == 0)
				ani = SOPHIA_ANI_MINI_IDLE_RIGHT;
			else
				ani = SOPHIA_ANI_MINI_WALKING_RIGHT;
		}
	}
	animationSet->at(ani)->Render(direction, x, y, alpha);
}

void MiniSophia::SetState(int state)
{
	Entity::SetState(state);

	switch (state)
	{
	case SOPHIA_MINI_STATE_WALKING_RIGHT:
		if (isCrawl)
			vx = SOPHIA_MINI_CRAWLING_SPEED;
		else if (isOnStair)
			vx = SOPHIA_MINI_GOSTAIR_SPEED;
		else
			vx = SOPHIA_MINI_WALKING_SPEED;
		direction = 1;
		break;
	case SOPHIA_MINI_STATE_WALKING_LEFT:
		if (isCrawl)
			vx = -SOPHIA_MINI_CRAWLING_SPEED;
		else if (isOnStair)
			vx = -SOPHIA_MINI_GOSTAIR_SPEED;
		else
			vx = -SOPHIA_MINI_WALKING_SPEED;
		direction = -1;
		break;
	case SOPHIA_MINI_STATE_CRAWL:
		if (!isJumping)
		{
			if (isCrawl)
			{
				y -= (SOPHIA_MINI_BBOX_HEIGHT - SOPHIA_MINI_CRAWL_BBOX_HEIGHT);
				vx = 0;
				isCrawl = false;
			}
			else
			{
				isCrawl = true;
			}
		}
		break;
	case SOPHIA_MINI_STATE_JUMP:
		isPressJump = true;
		if (isCrawl)
			return;

		if (isJumping)
			return;
		else
		{
			sound->Stop(GSOUND::S_JUMP);
			sound->Play(GSOUND::S_JUMP, false);
			isJumpHandle = false;
			isJumping = true;
			vy = -SOPHIA_MINI_JUMP_SPEED_Y;
		}
		break;
	case SOPHIA_MINI_STATE_IDLE:
		isPressJump = false;
		isOnStair = false;
		if (!isCrawl)
		{
			if (vx > 0) {
				vx -= SOPHIA_MINI_WALKING_ACC * dt;
				if (vx < 0)
					vx = 0;
			}
			else if (vx < 0) {
				vx += SOPHIA_MINI_WALKING_ACC * dt;
				if (vx > 0)
					vx = 0;
			}
		}
		break;
	case SOPHIA_MINI_STATE_CRAWL_STOP:
		if (isCrawl)
			vx = 0;
		break;
	case SOPHIA_MINI_STATE_DIE:
	case SOPHIA_MINI_STATE_IN:
	case SOPHIA_MINI_STATE_OUT:
		isCrawl = false;
		if (direction > 0) {
			vx -= 3 * SOPHIA_MINI_WALKING_ACC * dt;
			if (vx < 0)
				vx = 0;
		}
		else if (direction < 0) {
			vx += 3 * SOPHIA_MINI_WALKING_ACC * dt;
			if (vx > 0)
				vx = 0;
		}
		vy = -SOPHIA_MINI_JUMP_SPEED_Y/1.5f;
		break;
	case SOPHIA_MINI_STATE_UP_STAIR:
		if (isOnStair)
			vy = -SOPHIA_MINI_JUMP_SPEED_Y / 4;
		break;
	case SOPHIA_MINI_STATE_DOWN_STAIR:
		if (isOnStair)
			vy = SOPHIA_MINI_JUMP_SPEED_Y / 4;
		break;
	case SOPHIA_MINI_STATE_IDLE_STAIR:
		if (isOnStair)
		{
			vx = 0;
			vy = 0;
		}
		break;
	}
}

void MiniSophia::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!isDoneDeath)
	{
		if (isCrawl)
		{
			left = x;
			top = y;
			right = x + SOPHIA_MINI_CRAWL_BBOX_WIDTH;
			bottom = y + SOPHIA_MINI_CRAWL_BBOX_HEIGHT;
		}
		else
		{
			left = x;
			top = y;
			right = x + SOPHIA_MINI_BBOX_WIDTH;
			bottom = y + SOPHIA_MINI_BBOX_HEIGHT;
		}
	}
}

void MiniSophia::Reset()
{
	SetState(SOPHIA_MINI_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}
