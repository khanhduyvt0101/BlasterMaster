#include "Skulls.h"

void Skulls::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isDeath == false)
	{
		left = x;
		top = y;
		right = x + SKULLS_BBOX_WIDTH;
		bottom = y + SKULLS_BBOX_HEIGHT;
	}
}

Skulls::Skulls(float x, float y, LPGAMEENTITY t)
{
	enemyType = SKULLS;
	this->x = x;
	this->y = y;
	this->target = t;
	health = SKULLS_MAXHEALTH;
	isActive = false;
	isfly = 1;
	this->nx = -1;
	isfly = 1;
	isDamaged = 0;
	alpha = 255;
}



void Skulls::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	if (health <= 0)
		return;
	Entity::Update(dt);

	Move();
	
	
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
	if (state != SKULLS_STATE_DIE)
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

		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.1f;

		for (int i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult.at(i);
			if (e->nx != 0)
				this->direction = -this->direction;
		}

	}
#pragma endregion
	for (int i = 0; i < bullet.size(); i++)
	{
		if (target->IsCollidingObject(bullet.at(i)))
		{
			CheckBulletEnemy = 1;
			bullet.at(i)->_isFinish = 1;
		}
		bullet.at(i)->Update(dt, coObjects);
		if (bullet.at(i)->IsFinish())
			bullet.erase(bullet.begin() + i);
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Skulls::Render()
{
	if (isDeath)
		return;
	int ani;
	if (health <= 0)
	{
		ani = SKULLS_ANI_DIE;
		animationSet->at(ani)->OldRender(x+3, y+4);
	
		if (animationSet->at(ani)->GetFrame() == 3)
			SetState(SKULLS_STATE_DIE);
	}
	else
	{
		if (vx > 0)
			direction = 1;
		else if (vx < 0)
			direction = -1;
		ani = SKULLS_ANI_FLYRIGHT;
		animationSet->at(ani)->Render(direction, x, y);
	}
	for (int i = 0; i < bullet.size(); i++)
	{
		bullet.at(i)->Render();

	}
	//RenderBoundingBox();
}


bool Skulls::CheckRun()
{
	D3DXVECTOR2 posme;
	D3DXVECTOR2	posTarget;
	RECT BoxTarget = target->GetBBox();
	SetCenterBoundingBox(posme.x, posme.y, x, y, x + SKULLS_BBOX_WIDTH, y + SKULLS_BBOX_HEIGHT);
	SetCenterBoundingBox(posTarget.x, posTarget.y, BoxTarget.left, BoxTarget.top, BoxTarget.right, BoxTarget.bottom);
	return posme.x > (posTarget.x - 10) && posme.x < (posTarget.x + 10);
}

void Skulls::Move()
{
	
	if (CheckRun())
	{
		if (StateDelayTimer->IsTimeUp() && isfly)
		{
			isfly = 0;
			StateDelayTimer->Reset();
			StateDelayTimer->Start();
			directY = randomdirectY();
		}
		else if (StateDelayTimer->IsTimeUp() && !isfly)
		{
			isfly = 1;
			StateDelayTimer->Reset();
			StateDelayTimer->Start();
			directY = randomdirectY();
		}
	}

	SetState(SKULLS_STATE_FLYRIGHT);
}

void Skulls::Attack()
{
	bullet.push_back(new BulletSkulls(x+10, y+10, vx));
	isDamaged = 1;
}

void Skulls::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case SKULLS_STATE_DIE:
		
		vx = 0;
		vy = 0;
		isDeath = true;
		break;
	case SKULLS_STATE_FLYRIGHT:
		if (isfly)
		{
			if (this->direction > 0)
				vx = SKULLS_FLY_SPEED;
			else
				vx = -SKULLS_FLY_SPEED;
		}
		else
		{
			vx = 0;
			if (directY)
				vy -= SKULLS_GRAVITY;
			else
				vy += SKULLS_GRAVITY;
			
		}
		if (CheckRun())
		{
			if (DelayShot->IsTimeUp())
			{
				Attack();
				DelayShot->Reset();
				DelayShot->Start();
			}

		}
		break;
	case SKULLS_STATE_ATTACK:
		//Attack();
		break;
	}
}

void Skulls::Activation()
{

}

int Skulls::randomdirectY()
{
	random_device rm;
	mt19937 t(rm());
	uniform_int_distribution<int>	randi(1, 3);
	if (randi(t) == 2)
		return 1;
	else
		return -1;
}