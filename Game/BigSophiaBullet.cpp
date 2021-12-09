#include "BigSophiaBullet.h"
#include "math.h"
#include "Sound.h"
#define PI 3.14159265

BigSophiaBullet::BigSophiaBullet()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_BIG_SOPHIA_BULLET));
	alpha = 0;
	isCollisionBrick = 0;
	isCollisionEnemies = 0;
	isDone = true;
	damage = 5;
	timeDelayed = 0;
	timeDelayMax = BULLET_DELAY;
}

BigSophiaBullet::~BigSophiaBullet() {}

void BigSophiaBullet::Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects)
{
	//DebugOut(L"toa do y %f \n", y);
	totalTime += 20;
	if (isDone == true)
		alpha = 0;
	else
	{
		timeDelayed += dt;
		Entity::Update(dt);
		if (damage <= 3)
		{
			if (direction != 0)
			{
				vx = BULLET_SPEED * direction;
				vy = 0;
			}
			if (directionY != 0)
			{
				vy = BULLET_SPEED * directionY;
				vx = 0;
			}
		}
		else if (damage > 3 && damage <=5)
		{
			if (direction != 0)
			{
				vx = BULLET_SPEED * direction;
				if ((y + dy - oldY) > 30)
					vy = -0.6;
				else if ((oldY - y - dy) > 25)
					vy = 0.6;
			}
			if (directionY != 0)
			{
				vy = BULLET_SPEED * directionY;
				if ((x + dx - oldX) > 30)
					vx = -0.6;
				else if ((oldX - x - dx) > 25)
					vx = 0.6;
			}
		}
		else if (damage > 5)
		{
			if (direction != 0)
			{
				vx = BULLET_SPEED * direction;
				if ((y + dy - oldY) > 30)
					vy = -0.6;
				else if ((oldY - y - dy) > 25)
					vy = 0.6;
				//DebugOut(L"y-oldy %f  ", (abs(y + dy - oldY)));
				//DebugOut(L"vy  %f \n", vy);
			}
			if (directionY != 0)
			{
				vy = BULLET_SPEED * directionY;
				if ((x + dx - oldX) > 30)
					vx = -0.6;
				else if ((oldX - x - dx) > 25)
					vx = 0.6;
				//DebugOut(L"x-oldy %f \n", (abs(x + dx - oldX)));
				//DebugOut(L"vy  %f \n", vx);
				//DebugOut(L"total Time %f", sin(totalTime));
			}
		}
		

#pragma region Xử lý va chạm
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(colliable_objects, coEvents);

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
					sound->Reset(GSOUND::S_BULLET_EXPLODE);
					sound->Play(GSOUND::S_BULLET_EXPLODE, false);
					isCollisionBrick = 1;
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
					vx = 0;
					vy = 0;
				}
				if (e->obj->GetType() == EntityType::ENEMY)
				{
					sound->Reset(GSOUND::S_BULLET_EXPLODE);
					sound->Play(GSOUND::S_BULLET_EXPLODE, false);
					if (e->obj->isLavar == true)
						return;
					e->obj->AddHealth(-damage);
					isCollisionEnemies = 1;
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
					vx = 0;
					vy = 0;
				}

			}
		}
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
	}
}

void BigSophiaBullet::Render()
{
	//RenderBoundingBox();
	int ani;
	if (timeDelayed >= timeDelayMax)
	{
		/*vx = 0;
		ani = BULLET_JASON_BANG_ANI;
		animationSet->at(ani)->OldRender(x - DISTANCE_TO_BANG, y - DISTANCE_TO_BANG, alpha);
		if (animationSet->at(ani)->GetFrame() == 3)
		{
			isDone = true;
			timeDelayed = 0;
		}*/
		isDone = true;
		timeDelayed = 0;
	}
	else
	{
		if (isCollisionBrick == 0 && isCollisionEnemies == 0)
		{
			if (direction != 0)
			{
				if (damage < 2)
					ani = BIG_SOPHIA_BULLET_JASON_ANI_RIGHT_BLACK;
				else ani = BIG_SOPHIA_BULLET_JASON_ANI_RIGHT_COLOR;
				//DebugOut(L"%d \n", ani);
				animationSet->at(ani)->Render(direction, x, y, alpha);
			}
			else if (directionY != 0)
			{
				if (damage < 2)
					ani = BIG_SOPHIA_BULLET_JASON_ANI_TOP_BLACK;
				else ani = BIG_SOPHIA_BULLET_JASON_ANI_TOP_COLOR;
				animationSet->at(ani)->RenderY(directionY, x, y, alpha); 
			}

		}
		else if (isCollisionEnemies == 1)
		{

			isDone = true;
			timeDelayed = 0;;
		}
		else if (isCollisionBrick == 1)
		{
			
			ani = BULLET_JASON_BANG_ANI;
			animationSet->at(ani)->OldRender(x - DISTANCE_TO_BANG, y - DISTANCE_TO_BANG, alpha);
			if (animationSet->at(ani)->GetFrame() == 3)
			{
				isDone = true;
				timeDelayed = 0;
			}
		}
	}
}

void BigSophiaBullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (!isDone)
	{
		l = x;
		t = y;
		if (direction == 0)
		{
			r = x + BIG_SOPHIA_BULLET_BBOX_WIDTH;
			b = y + BIG_SOPHIA_BULLET_BBOX_HEIGHT;
		}
		else
		{
			r = x + BIG_SOPHIA_BULLET_BBOX_HEIGHT;
			b = y + BIG_SOPHIA_BULLET_BBOX_WIDTH;
		}
	}
}