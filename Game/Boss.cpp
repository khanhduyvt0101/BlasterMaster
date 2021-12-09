#include "Boss.h"

void Boss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!isDeath)
	{
		left = x;
		top = y;
		right = x + BOSS_BBOX_WIDTH;
		bottom = y + BOSS_BBOX_HEIGHT;
	}
	tempX = (left + right) / 2;
	tempY = (top + bottom) / 2;
	PosBoss.left = x;
	PosBoss.top = y;
	PosBoss.right = x + BOSS_BBOX_WIDTH;
	PosBoss.bottom = y + BOSS_BBOX_HEIGHT;
}

Boss::Boss(float x, float y, LPGAMEENTITY t)
{
	enemyType = BOSS;
	this->x = x;
	this->y = y;
	target = t;
	isActive = false;
	isDamaged = false;
	isDeath = 0;
	isDie = 0;
	health = BOSS_MAXHEALTH;
	SetState(BOSS_STATE_FLY);
	ListHand.push_back(new BossHand(x, y, 2, -1));
	for (int  i = 1; i < 5; i++)
	{
		ListHand.push_back(new BossHand(ListHand.at(i-1)->GetPos().left, ListHand.at(i-1)->GetPos().top, 1, -1));
		
	}

	ListHand.push_back(new BossHand(x + BOSS_BBOX_WIDTH - 18, y-5, 2, 1));
	for (int i = 6; i < 10; i++)
	{
		ListHand.push_back(new BossHand(ListHand.at(i - 1)->GetPos().left, ListHand.at(i - 1)->GetPos().top, 1, -1));
	}

}

void Boss::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	if (health <= 0)
		return;
	Entity::Update(dt);

#pragma region Xử lý đường bay
	int _x, _y;
	Camera::GetInstance()->GetCenter(_x, _y);
	if (TimeDelayY->IsTimeUp())
		if ((this->tempY <= _y - SCREEN_HEIGHT / 2.5) || (this->tempY >= _y - BOSS_BBOX_HEIGHT))
		{
			
			vy = -vy;
			TimeDelayY->Reset();
			TimeDelayY->Start();

		}
	if (TimeDelayX->IsTimeUp())
		if ((this->tempX <= _x - SCREEN_WIDTH / 2.5) || (this->tempX >= _x + SCREEN_WIDTH / 3))
		{
			vx = -vx;
			TimeDelayX->Reset();
			TimeDelayX->Start();
		}
#pragma endregion

#pragma region Xử lý bullet
	if (DelayTime->IsTimeUp())
	{
		Attack();
		delayTimeranishot->Reset();
		delayTimeranishot->Start();
		//SetState(BOSS_STATE_ATTACK);
		DelayTime->Reset(randomTimeAttack());
		DelayTime->Start();
	}
	else
	{
		//SetState(BOSS_STATE_FLY);
	}
	for (int i = 0; i < bullet.size(); i++)
	{
		if (target->IsCollidingObject(bullet.at(i)))
		{
			CheckBulletEnemy = 1;
		}
		bullet.at(i)->Update(dt, coObjects);
		if (bullet.at(i)->IsFinish())
			bullet.erase(bullet.begin() + i);
	}

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
	if (state != BOSS_STATE_DIE)
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

		if (nx!=0)
		{
			vx = -vx;
		}
		if (ny!=0)
		{
			vy = -vy;
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
	D3DXVECTOR2 Speed(vx, vy);



	ListHand.at(0)->Update(dt, coObjects, PosBoss, Speed, Speed, GetBBox());
	for (int i = 1; i < 5; i++)
	{
		
		ListHand.at(i)->Update(dt, coObjects, ListHand.at(i-1)->GetPos(), ListHand.at(i-1)->GetSpeed(), Speed, GetBBox());
	}

	ListHand.at(5)->Update(dt, coObjects, PosBoss, Speed, Speed, GetBBox());

	for (int i = 6; i < 10; i++)
	{
		ListHand.at(i)->Update(dt, coObjects, ListHand.at(i - 1)->GetPos(), ListHand.at(i - 1)->GetSpeed(), Speed, GetBBox());
	}
	
}

void Boss::Render()
{
	if (isDeath)
		return;
	int ani;
	
	if (isDie && TimeDie->IsTimeUp())
	{
		SetState(BOSS_STATE_DIE);
	}

	if (health <= 0 && health >-10)
	{
		ani = BOSS_ANI_DIE;
		animationSet->at(ani)->OldRender(x, y);
		TimeDie->Start();
		health -= 1000;
		
	}
	else if(!TimeDie->IsTimeUp())
	{
		ani = BOSS_ANI_DIE;
		animationSet->at(ani)->OldRender(x, y);
		isDie = 1;
	}
	else if (delayTimeranishot->IsTimeUp() && health > 0)
	{
		ani = BOSS_ANI_FLY;
		animationSet->at(ani)->OldRender(x, y);

	}
	else if (!delayTimeranishot->IsTimeUp() && health > 0)
	{
		ani = BOSS_ANI_ATTACK;
		animationSet->at(ani)->OldRender(x, y);

	}

	if (!isDie)
	{
		for (int i = 0; i < ListHand.size(); i++)
		{
			ListHand.at(i)->Render();
		}

		for (int i = 0; i < bullet.size(); i++)
		{
			bullet.at(i)->Render();
		}
		
	}
	//RenderBoundingBox();
}



void Boss::SetState(int state)
{
	Entity::SetState(state);

	switch (state)
	{
	case BOSS_STATE_DIE:
	{
		vx = 0;
		vy = 0;
		isDeath = 1;
		break;
	}
	case BOSS_STATE_FLY:
	{
		vx = BOSS_SPEED_VX;
		vy = BOSS_SPEED_VY;

	}
	case BOSS_STATE_ATTACK:
		DelayTime->Reset(randomTimeAttack());
		DelayTime->Start();


		break;
	default:
		break;
	}
}

void Boss::Activation()
{

}

void Boss::Attack()
{
	RECT player = target->GetBBox();
	bullet.push_back(new BulletBoss(x, y, x + BOSS_BBOX_WIDTH, y + BOSS_BBOX_HEIGHT, player.left, player.top, player.right, player.bottom));
	isDamaged = 1;
}

int Boss::randomTimeAttack()
{
	random_device rm;
	mt19937 t(rm());
	uniform_int_distribution<int>	randi(1, BOSS_MAXTIME_ATTACK);
	return randi(t);
}
