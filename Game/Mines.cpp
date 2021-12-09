#include "Mines.h"

void Mines::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!isDeath)
	{
		left = x;
		top = y;
		right = x + MINES_BBOX_WIDTH;
		bottom = y + MINES_BBOX_HEIGHT;
	}
}

Mines::Mines(float x, float y, LPGAMEENTITY t)
{
	enemyType = MINES;
	this->x = x;
	this->y = y;
	this->target = t;
	isDeath = false;
	health = MINES_MAXHEALTH;
	isActive = false;
	CheckColli = 0;
	isDamaged = 0;
}

void Mines::Attack()
{
	bullet.push_back(new BulletMines(x + 8, y));
	isDamaged = 1;
}

int Mines::RandomAttack()
{
	random_device rm;
	mt19937 t(rm());
	uniform_int_distribution<int>	randi(2, 3);
	return randi(t);
}

void Mines::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	/*if (health <= 0)
		return;*/
	Entity::Update(dt);
	SetState(MINES_STATE_READY);
	vy += MINES_GRAVITY * dt;

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
	if (state != MINES_STATE_DIE)
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

		if (CheckColisionEnemy && !CheckColli)
		{
			/*SetState(MINES_STATE_DIE);*/
			health = 0;
			for (int i = 0; i < RandomAttack(); i++)
				Attack();
			CheckColli = 1;
		}

	}
	for (int i = 0; i < bullet.size(); i++)
	{
		if (target->IsCollidingObject(bullet.at(i)))
		{
			CheckBulletEnemy = 1;
			bullet.at(i)->_isFinish = 1;
		}
		bullet.at(i)->Update(dt, coObjects);
		if (bullet.at(i)->IsFinish())
		{
			bullet.erase(bullet.begin() + i);
			isDeath = 1;
		}
	}
	
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	if (health <= 0 && !CheckColisionEnemy && !isDamaged)
	{
		for (int i = 0; i < RandomAttack(); i++)
			Attack();
	}
	/*else if (health == -1 && !CheckColisionEnemy)
	{
		for (int i = 0; i < RandomAttack(); i++)
			Attack();
	}*/
#pragma endregion
}

void Mines::Render()
{
	if (isDeath)
		return;
	int ani;
	if (health <= 0 && health >= -10)
	{
		ani = MINES_ANI_DIE;
		animationSet->at(ani)->OldRender(x+2, y+1);
		//DebugOut(L"dsadasdasd %d ", animationSet->at(ani)->GetFrame());
		if (animationSet->at(ani)->GetFrame() == 3)
			health = -1000;
	}
	else if (health == -1000)
	{
		//RenderBoundingBox();
	}
	else
	{
		ani = MINES_ANI_READY;
		animationSet->at(ani)->OldRender(x, y);
	}
	//RenderBoundingBox();
	for (int i = 0; i < bullet.size(); i++)
	{
		bullet.at(i)->Render();

	}
}

void Mines::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case MINES_STATE_DIE:
		vx = 0;
		vy = 0;
		isDeath = true;
		break;
	case MINES_STATE_READY:
		vx = 0;
		vy = 0;
		break;
	}
}

void Mines::Activation()
{
}


