#include "Eyeballs.h"
#include "define.h"

void Eyeballs::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!isDeath)
	{
		left = x;
		top = y;
		right = x + EYEBALLS_BBOX_WIDTH;
		bottom = y + EYEBALLS_BBOX_HEIGHT;
	}
}

Eyeballs::Eyeballs(float x, float y, LPGAMEENTITY t, int _species)
{

	enemyType = EYEBALLS;

	this->target = t;
	health = EYEBALLS_MAXHEALTH;
	isActive = false;
	isDamaged = false;
	isDeath = 0;
	species = _species;
	if (_species == 100)
	{
		RenderSpeedFollowTarget(1);

		this->x = x;
		this->y = y;
		TimeFly->Start();
		fly = 1;
		/*SetXYCircle();*/
		SetState(EYEBALLS_STATE_FLY_FOLLOW);
	}
	else if (_species == 200)
	{
		SetXYCircle();
		randomSpeed();
	}
	else
	{
		SetState(EYEBALLS_STATE_FLY_CLIMBTOP);
		this->x = x;
		this->y = y;
	}


}

void Eyeballs::RenderSpeedFollowTarget(float _BULLET_SPEED)
{
	D3DXVECTOR2 posEyeballs;
	posEyeballs = D3DXVECTOR2(x, y);
	focus = CreatePosFollowTarget(D3DXVECTOR2(target->x, target->y), posEyeballs);
	posEyeballs = RadialMovement(focus, posEyeballs, _BULLET_SPEED);
	RenderVx = posEyeballs.x / 14;
	RenderVy = posEyeballs.y / 14;
}

D3DXVECTOR2 Eyeballs::CreatePosFollowTarget(D3DXVECTOR2 focus, D3DXVECTOR2 pos)
{
	D3DXVECTOR2 realtarget;
	D3DXVECTOR2 vectordirection = focus - pos;
	realtarget.x = focus.x + vectordirection.x * 1000;
	realtarget.y = focus.y + vectordirection.y * 1000;
	return realtarget;
}

void Eyeballs::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	if (health <= 0)
		return;
	Entity::Update(dt);

#pragma region Xử lý bullet

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

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEENTITY> bricks;

	coEvents.clear();
	bricks.clear();


	if (species == 200)
	{
#pragma region EyeBallsRandom
		int _x, _y;
		Camera::GetInstance()->GetCenter(_x, _y);
		if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(_x, _y)) >= SCREEN_HEIGHT)
		{
			SetXYCircle();
			randomSpeed();
		}
		SetState(EYEBALLS_STATE_FLY_RANDOM);
#pragma region Xử lý tiền va chạm


		for (UINT i = 0; i < coObjects->size(); i++)
			if (coObjects->at(i)->GetType() == EntityType::PLAYER)
				bricks.push_back(coObjects->at(i));

		// turn off collision when die
		if (state != EYEBALLS_STATE_DIE)
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


		}
#pragma endregion

	}
#pragma endregion


	else if (species == 100)
	{
#pragma region EyeBallsFollow

		SetState(EYEBALLS_STATE_FLY_FOLLOW);

#pragma region Xử lý tiền va chạm


		for (UINT i = 0; i < coObjects->size(); i++)
			if (coObjects->at(i)->GetType() == EntityType::BRICK)
				bricks.push_back(coObjects->at(i));

		// turn off collision when die
		if (state != EYEBALLS_STATE_DIE)
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

			if (species == 100)
			{
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
				if (nx != 0 || ny != 0)
				{
					if (species == EYEBALLS_STATE_FLY_FOLLOW)
						RenderSpeedFollowTarget(1);
				}
			}

		}
#pragma endregion
	}
#pragma endregion

	else
	{
#pragma region EyeBallsClimbWall
		SetState(EYEBALLS_STATE_FLY_CLIMBTOP);

#pragma region Xử lý tiền va chạm


		for (UINT i = 0; i < coObjects->size(); i++)
			if (coObjects->at(i)->GetType() == EntityType::BRICK)
				bricks.push_back(coObjects->at(i));

		// turn off collision when die
		if (state != EYEBALLS_STATE_DIE)
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
			if (TimeJump->IsTimeUp())
			{
				vy = EYEBALLS_SPEED_VY;
				vx = -EYEBALLS_SPEED_VX;
				TimeJump->Reset();
				TimeJump->Start();
				Attack();
				delayTimeranishot->Reset();
				delayTimeranishot->Start();
			}
			if (vy < 0)
			{
				vx = 0;
			}
			if (nx != 0)
			{
				vx = -5 * vx;
			}
			//DebugOut(L"\nvy:   %f", vy);

		}
#pragma endregion
	}

#pragma endregion

	Activation();
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}


void Eyeballs::Render()
{
	if (isDeath)
		return;
	int ani;
	if (health <= 0)
	{
		ani = EYEBALLS_ANI_DIE;
		animationSet->at(ani)->Render(direction, x+2, y+1);
		if (animationSet->at(ani)->GetFrame() == 2)
			SetState(EYEBALLS_STATE_DIE);
	}
	else if (!delayTimeranishot->IsTimeUp())
	{
		ani = EYEBALLS_ANI_ATTACK;
		animationSet->at(ani)->OldRender(x, y);

	}
	else
	{
		if (vx > 0)
			direction = 1;
		else
			direction = -1;
		ani = EYEBALLS_ANI_FLY;
		animationSet->at(ani)->Render(direction, x, y);
	}
	//RenderBoundingBox();
	for (int i = 0; i < bullet.size(); i++)
	{
		bullet.at(i)->Render();
	}
}



void Eyeballs::SetState(int state)
{
	Entity::SetState(state);

	switch (state)
	{
	case EYEBALLS_STATE_DIE:
		vx = 0;
		vy = 0;
		isDeath = 1;
		break;

	case EYEBALLS_STATE_FLY_FOLLOW:
	{
		if (TimeFly->IsTimeUp() && fly == 1)
		{
			TimeFly->Reset(EYEBALLS_TIME_DELAYFLY);
			TimeFly->Start();
			fly = 0;
		}
		else if (TimeFly->IsTimeUp() && fly == 0)
		{
			TimeFly->Reset(EYEBALLS_TIME_FLY);
			TimeFly->Start();
			fly = 1;
			Attack();
			delayTimeranishot->Reset();
			delayTimeranishot->Start();

		}
		else if (fly == 0 && !TimeFly->IsTimeUp())
		{
			vx = 0;
			vy = 0;
		}
		else
		{
			vx = RenderVx;
			vy = RenderVy;
		}


		break;
	}
	case EYEBALLS_STATE_FLY_RANDOM:
		vx = vxR * 2;
		vy = vyR * 2;

		break;
	case EYEBALLS_STATE_FLY_CLIMBTOP:
		vy -= EYEBALLS_GRAVITY;

		break;

	case EYEBALLS_STATE_ATTACK:



		break;

	default:
		break;
	}
}

void Eyeballs::Activation()
{
	if (!isActive)
	{
		vx = 0;
	}
	else
	{
		SetState(species);
	}
	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= EYEBALLS_SITEACTIVE_PLAYER)
	{
		isActive = true;
	}
}

void Eyeballs::SetXYCircle()
{

	int _x, _y;
	Camera::GetInstance()->GetCenter(_x, _y);

	random_device rm;
	mt19937 t(rm());
	//uniform_real_distribution<float>	randi1(_x - SCREEN_WIDTH, _x - SCREEN_WIDTH / 2); //Bên trái màn hình
	//uniform_real_distribution<float>	randi2(_x + SCREEN_WIDTH / 2, _x + SCREEN_WIDTH); //Bên phải màn hình
	uniform_real_distribution<float>	randi(_x - SCREEN_WIDTH / 2, _x + SCREEN_WIDTH / 2); // Phía giữa màn hình
	//uniform_real_distribution<float>	randj1(_y - SCREEN_HEIGHT, _y - SCREEN_HEIGHT/2); // Phía trên mh
	//uniform_real_distribution<float>	randj2(_y + SCREEN_HEIGHT / 2, _y + SCREEN_HEIGHT);  // Phía dưới mh
	uniform_real_distribution<float>	randj(_y - SCREEN_HEIGHT / 2, _y + SCREEN_HEIGHT / 2);  // Phía giữa mh
	uniform_int_distribution<int>	randk(1, 4);

	if (species == 100)
	{
		x = randi(t);
		y = randj(t);
	}
	else if (species == 200)
	{
		if (randk(t) == 1)
		{
			y = _y - SCREEN_HEIGHT;
			x = randi(t);
		}
		else if (randk(t) == 2)
		{
			y = _y + SCREEN_HEIGHT;
			x = randi(t);
		}
		else if (randk(t) == 3)
		{
			x = _x - SCREEN_WIDTH;
			y = randj(t);
		}
		else
		{
			x = _x + SCREEN_WIDTH;
			y = randj(t);
		}
	}

}

void Eyeballs::randomSpeed()
{
	random_device rm;
	mt19937 t(rm());
	uniform_real_distribution<float> rand(0, EYEBALLS_SPEED_MAXVY);
	uniform_int_distribution<int>	randi(1, 6);
	uniform_int_distribution<int>	randj(1, 6);
	float res;
	for (int i = 0; i < randi(t); ++i) {
		if (i % 2 == 0)
			res = rand(t);
		else
			res = -rand(t);
	}
	for (int i = 0; i < randj(t); ++i)
	{
		if (i % 2 == 0)
			vxR = EYEBALLS_SPEED_CONSTVX;
		else
			vxR = -EYEBALLS_SPEED_CONSTVX;
	}
	vyR = res * vxR;
}

void Eyeballs::Attack()
{
	RECT player = target->GetBBox();
	bullet.push_back(new BulletEyeballs(x, y, x + EYEBALLS_BBOX_WIDTH, y + EYEBALLS_BBOX_HEIGHT, player.left, player.top, player.right, player.bottom));
	isDamaged = 1;
}
