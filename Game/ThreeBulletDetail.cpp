#include "ThreeBulletDetail.h"
#include "Sound.h"


ThreeBulletDetail::ThreeBulletDetail()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(8));
	x = 0;
	y = 0;
	alpha = 0;
	isCollisionBrick = 0;
	isCollisionEnemies = 0;
	isDone = true;
	damage = 2;
	timeDelayed = 0;
	timeDelayMax = SMALL_JASON_BULLET_DELAY;
}

ThreeBulletDetail::~ThreeBulletDetail() {}

void ThreeBulletDetail::Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects)
{
	if (isThreeBullet != 0)
	{
		vx = 0.15f * direction;
		if (isThreeBullet == 1)
		{
			if (abs(y - oldY) <= 25)
			{
				vy = -0.05f;

			}
			else
			{
				vy = 0;
			}
		}
		else if (isThreeBullet == 2)
		{
			vy = 0;
		}
		else if (isThreeBullet == 3)
		{
			if (abs(y - oldY) <= 30)
			{
				vy = 0.05f;
			}
			else
			{
				vy = 0;
			}
		}
		Entity::Update(dt);
		x += dx;
		y += dy;
		vector<LPCOLLISIONEVENT> coEvents;
		coEvents.clear();

		CalcPotentialCollisions(colliable_objects, coEvents);
		/*for (int i = 0; i < coEvents.size(); i++)
		{
			if (coEvents[i]->obj->GetType() == EntityType::ENEMY)
			{
				if (this->IsCollidingObject(coEvents[i]->obj))
				{
					coEvents[i]->obj->AddHealth(-damage);
					isCollisionEnemies = 1;
					vx = 0;
					vy = 0;
				}
			}
		}*/
		for (UINT i = 0; i < colliable_objects->size(); i++)
		{
			if (colliable_objects->at(i)->GetType() == EntityType::ENEMY)
			{

				if (this->IsCollidingObject(colliable_objects->at(i)))
				{
					colliable_objects->at(i)->AddHealth(-damage);
					if (isCollisionEnemies != 1)
					{
						sound->Reset(GSOUND::S_BULLET_EXPLODE);
						sound->Play(GSOUND::S_BULLET_EXPLODE, false);
					}
					isCollisionEnemies = 1;
					
					vx = 0;
					vy = 0;
				}
			}
		}

	}
	else
	{
		if (typeBullet == 0)
			damage = 1;
		else
			damage = 2;
		if (isDone == true)
		{
			alpha = 0;
		}
		else
		{
			timeDelayed += dt;
			Entity::Update(dt);
			if (isTargetTop == false) {
				vx = BULLET_SPEED * direction;
				vy = 0;
			}
			else {
				vy = -BULLET_SPEED;
				vx = 0;
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
						isCollisionBrick = 1;
						x += min_tx * dx + nx * 0.4f;
						y += min_ty * dy + ny * 0.4f;
						vx = 0;
						vy = 0;
					}
					if (e->obj->GetType() == EntityType::ENEMY)
					{
						e->obj->AddHealth(-damage);
						DebugOut(L"xxxxxxxxxxxxxxxx %d", e->obj->health);
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
}

void ThreeBulletDetail::Render()
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
		isCountBack = false;
	}
	else
	{
		if (isCollisionBrick == 0 && isCollisionEnemies == 0)
		{
			ani = 0;
			animationSet->at(ani)->Render(direction, x, y, alpha);
		}
		else if (isCollisionEnemies == 1)
		{
			isDone = true;
			timeDelayed = 0;;
			isCountBack = false;
		}
		else if (isCollisionBrick == 1)
		{
			ani = 1;
			if (!isCountBack)
			{
				animationSet->at(ani)->ResetCurrentFrame();
				isCountBack = true;
			}
			animationSet->at(ani)->OldRender(x - DISTANCE_TO_BANG, y - DISTANCE_TO_BANG, alpha);
			if (animationSet->at(ani)->GetFrame() == 3)
			{
				isDone = true;
				timeDelayed = 0;
				isCountBack = false;
			}
		}
	}
}

void ThreeBulletDetail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (!isDone)
	{
		l = x;
		t = y;
		if (typeBullet == 0)
		{
			r = x + MINI_BULLET_BBOX_WIDTH;
			b = y + MINI_BULLET_BBOX_HEIGHT;
		}
		else
		{
			if (isTargetTop == false)
			{
				r = x + THREE_BULLET_BBOX_WIDTH;
				b = y + THREE_BULLET_BBOX_HEIGHT;
			}
			else
			{
				r = x + THREE_BULLET_BBOX_HEIGHT;
				b = y + THREE_BULLET_BBOX_WIDTH;
			}
		}
	}
}