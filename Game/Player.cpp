#include <algorithm>
#include <assert.h>
#include "debug.h"
#include "Player.h"
#include "Game.h"
#include "Sound.h"

Player::Player(float x, float y) : Entity()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_PLAYER));
	untouchable = 0;
	SetState(SOPHIA_STATE_IDLE);
	tag = PLAYER;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	backup_JumpY = 0;
	gunDam = MAX_HEALTH;
	health = MAX_HEALTH;

	isImmortaling = false;

	isDeath = false;
	alpha = 255;

}
Player* Player::instance = NULL;
Player* Player::GetInstance()
{
	if (instance == NULL)
		instance = new Player();
	return instance;
}

void Player::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects, vector<LPGAMEENTITY>* coEnemies)
{

	//DebugOut(L"x: %f \n", x);
	//DebugOut(L"x: %f \n", y);
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
	vy += SOPHIA_GRAVITY * dt;
	if (isJumping && backup_JumpY - y >= HIGHT_LEVER1 && isJumpHandle == false) 
	{
		if (!isPressJump)
			vy = 0;
		isJumpHandle = true;
	}
#pragma endregion

#pragma region Xử lý gun flip
	if (isPressFlipGun == false)
	{
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_1)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_2)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_3)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_4)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_1)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_2)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_3)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_4)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_RIGHT)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_LEFT)->ResetCurrentFrame();
	}
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
	if (state != SOPHIA_STATE_DIE)
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
				y += min_ty * dy + ny * 0.005f;	
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

	/*for (int i = 0; i < bullet.size(); i++)
	{
		bullet.at(i)->Update(dt, coObjects);
		if (bullet.at(i)->IsFinish())
			bullet.erase(bullet.begin() + i);
	}*/
#pragma endregion
}

void Player::SetInjured(int dame)
{
	if (isImmortaling)
		return;
	sound->Play(GSOUND::S_HEALTH, false);
	if (health > 1)
		health -= dame;
	if (gunDam > 1)
		gunDam -= dame;
	StartUntouchable();
	immortalTimer->Start();
	isImmortaling = true;
}

void Player::Render()
{/*
	for (int i = 0; i < bullet.size(); i++)
	{
		bullet.at(i)->Render();
		DebugOut(L"Render");
	}*/
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
		ani = SOPHIA_JASON_ANI_DIE;
		animationSet->at(ani)->Render(direction,x - DISTANCE_X_TO_DIE, y - DISTANCE_Y_TO_DIE, alpha);
		if (animationSet->at(ani)->GetFrame() > FRAME_LAST_DIE)
		{
			//DebugOut(L"[frame]: %d;\n", animation_set->at(SOPHIA_ANI_JASON_WALKING_RIGHT)->GetFrame());
			isDoneDeath = true;
		}
	}
	else if (isOpening)
	{
		ani = SOPHIA_JASON_ANI_GET_OUT;
		animationSet->at(ani)->Render(direction, x, y - DISTANCE_TO_OUT, alpha);
		if (animationSet->at(ani)->GetFrame() >= 1)
			isOpening = false;
	}
	else if (isPressFlipGun == true)
	{
		if (vx == 0)
		{
			if (direction > 0)
			{
				//idle theo walking
				current_frame = animationSet->at(SOPHIA_ANI_JASON_WALKING_RIGHT)->GetFrameStopWalking();
				switch (current_frame)
				{
				case SOPHIA_STOP_WALKING_SPRITE2:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_2;
					break;
				case SOPHIA_STOP_WALKING_SPRITE3:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_3;
					break;
				case SOPHIA_STOP_WALKING_SPRITE4:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_4;
					break;
				default:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_1;
					break;
				}
			}
			else
			{
				current_frame = animationSet->at(SOPHIA_ANI_JASON_WALKING_LEFT)->GetFrameStopWalking();
				switch (current_frame)
				{
				case SOPHIA_STOP_WALKING_SPRITE2:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_2;
					break;
				case SOPHIA_STOP_WALKING_SPRITE3:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_3;
					break;
				case SOPHIA_STOP_WALKING_SPRITE4:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_4;
					break;
				default:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_1;
					break;
				}
			}

			if (isGunFlipping == false)
			{
				animationSet->at(ani)->OldRender(x, y - SOPHIA_JASON_HEIGHT_GUN_FLIP, alpha);
				if (animationSet->at(ani)->GetFrame() == 1)
				{
					//DebugOut(L"[frame]: %d;\n", animation_set->at(SOPHIA_ANI_JASON_WALKING_RIGHT)->GetFrame());
					isGunFlipping = true;
				}
				return;

			}
			else
			{
				animationSet->at(ani)->RenderFrame(1, x, y - SOPHIA_JASON_HEIGHT_GUN_FLIP, alpha);
				return;
			}
		}
		else if (vx > 0)
			ani = SOPHIA_ANI_GUN_FLIP_RIGHT;
		else
			ani = SOPHIA_ANI_GUN_FLIP_LEFT;

		if (isGunFlipping == false)
		{
			animationSet->at(ani)->RenderGunFlip(x, y - SOPHIA_JASON_HEIGHT_GUN_FLIP, alpha);
			if (animationSet->at(ani)->GetFrame() > FRAME_LAST_GUNFLIP)
			{
				//DebugOut(L"[frame]: %d;\n", animation_set->at(SOPHIA_ANI_JASON_WALKING_RIGHT)->GetFrame());
				isGunFlipping = true;
			}
			return;
		}
		else
		{
			animationSet->at(ani)->RenderGunFlipTargetTop(x, y - SOPHIA_JASON_HEIGHT_GUN_FLIP, alpha);
			return;
		}
	}
	else 
	{
		if (isJumping == false)
		{
			if (vx == 0)
			{
				if (direction > 0)
				{
					ani = SOPHIA_ANI_JASON_WALKING_RIGHT;
					current_frame = animationSet->at(ani)->GetFrameStopWalking();
					//DebugOut(L"[frame]: %d;\n", animation_set->at(SOPHIA_ANI_JASON_WALKING_RIGHT)->GetFrameStopWalking());
				}
				else
				{
					ani = SOPHIA_ANI_JASON_WALKING_LEFT;
					current_frame = animationSet->at(ani)->GetFrameStopWalking();
				}
				animationSet->at(ani)->RenderFrame(current_frame, x, y, alpha);// ve frame idle
				isGunFlipping = false;
				return;
			}
			else if (vx > 0)
				ani = SOPHIA_ANI_JASON_WALKING_RIGHT;
			else ani = SOPHIA_ANI_JASON_WALKING_LEFT;
			animationSet->at(ani)->OldRender(x, y, alpha);
			isGunFlipping = false;
		}
		else
		{
			if (vx == 0 && vy > 0)
			{
				if (direction > 0) ani = SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_RIGHT;
				else ani = SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_LEFT;
			}
			else if (vx == 0 && vy <= 0)
			{
				if (direction > 0) ani = SOPHIA_ANI_JASON_JUMP_UP_IDLE_RIGHT;
				else ani = SOPHIA_ANI_JASON_JUMP_UP_IDLE_LEFT;
			}
			else if (vy > 0)
			{
				if (vx > 0) ani = SOPHIA_ANI_JASON_JUMP_DOWN_WALKING_RIGHT;
				else ani = SOPHIA_ANI_JASON_JUMP_DOWN_WALKING_LEFT;
			}
			else if (vy <= 0)
			{
				if (vx > 0) ani = SOPHIA_ANI_JASON_JUMP_UP_WALKING_RIGHT;
				else ani = SOPHIA_ANI_JASON_JUMP_UP_WALKING_LEFT;
			}
			isGunFlipping = false;
			animationSet->at(ani)->OldRender(x, y, alpha);
			return;
		}
	}
}

void Player::SetState(int state)
{
	Entity::SetState(state);

	switch (state)
	{
	case SOPHIA_STATE_WALKING_RIGHT:
		vx = SOPHIA_WALKING_SPEED;
		direction = 1;
		break;
	case SOPHIA_STATE_WALKING_LEFT:
		vx = -SOPHIA_WALKING_SPEED;
		direction = -1;
		break;
	case SOPHIA_STATE_JUMP:
		isPressJump = true;
		if (isJumping == true)
			return;
		else
		{
			sound->Stop(GSOUND::S_JUMP);
			sound->Play(GSOUND::S_JUMP, false);
			isJumpHandle = false;
			isJumping = true;
			vy = -SOPHIA_JUMP_SPEED_Y;
			backup_JumpY = y;
		}
		break;
	case SOPHIA_STATE_IDLE:
		isPressJump = false;
		if (vx > 0) {
			vx -= SOPHIA_WALKING_ACC * dt;
			if (vx < 0)
				vx = 0;
		}
		else if (vx < 0) {
			vx += SOPHIA_WALKING_ACC * dt;
			if (vx > 0)
				vx = 0;
		}
		break;
		isPressJump = false;
	case SOPHIA_STATE_IDLE2:
		vx = 0;
		break;
	case SOPHIA_STATE_OUT:
		vx = 0;
		isOpening = true;
		break;
	}
}

void Player::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isDoneDeath == false)
	{
		left = x;
		top = y;
		right = x + SOPHIA_JASON_BBOX_WIDTH;
		bottom = y + SOPHIA_JASON_BBOX_HEIGHT;
	}
}

void Player::Reset()
{
	health = MAX_HEALTH;
	isDoneDeath = false;
	isDeath = false;
	SetState(SOPHIA_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}


void Player::Attack()
{
	sound->Reset(GSOUND::S_BULLET_SOPHIA);
	sound->Play(GSOUND::S_BULLET_SOPHIA, false);
	//bullet.push_back(new BulletSkulls(x + 20, y - 20, 0.1*direction));
}

