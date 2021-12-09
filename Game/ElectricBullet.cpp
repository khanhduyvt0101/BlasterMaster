#include "ElectricBullet.h"

ElectricBullet::ElectricBullet()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_ELECTRIC_BULLET));
	x = 0;
	y = 0;
	alpha = 0;
	isCollisionBrick = 0;
	timeDelayed = 0;
	timeDelayMax = ELECTRIC_DELAY;
}

ElectricBullet::~ElectricBullet() {}

void ElectricBullet::Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects)
{
	if (isDone == true)
	{
		alpha = 0;
		return;
	}
	else
	{
		timeDelayed += dt;
		Entity::Update(dt);

#pragma region Xử lý va chạm
		/*vector<LPCOLLISIONEVENT> coEvents;
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
				if (e->obj->GetType() == EntityType::GATE)
				{
					if (e->nx != 0)
					{
						isCollision = 1;
						x += min_tx * dx + nx * 0.4f;
						y += min_ty * dy + ny * 0.4f;
						vx = 0;
						vy = 0;
					}
				}
			}
		}
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];*/
#pragma endregion
	}
}

void ElectricBullet::Render()
{
	if (!isDone)
	{
		if (!isCountBack)
		{
			animationSet->at(0)->ResetCurrentFrame();
			isCountBack = true;
		}
		animationSet->at(0)->OldRender(x, y, alpha);
		if (animationSet->at(0)->GetFrame() == 9)
		{
			isDone = true;
			//timeDelayed = 0;
			isCountBack = false;
		}
		//RenderBoundingBox();
	}
}

void ElectricBullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (!isDone)
	{
		l = x;
		t = y + 13;
		r = x + ELECTRIC_BULLET_JASON_BBOX_WIDTH;
		b = y + ELECTRIC_BULLET_JASON_BBOX_HEIGHT + 13;
	}
}