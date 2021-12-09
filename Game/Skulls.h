#pragma once
#include "Enemy.h"
#include "Brick.h"
#include "Player.h"
#include "Timer.h"
#include "BulletSkulls.h"
#include <ctime>

#define SKULLS_FLY_SPEED					0.1f

#define SKULLS_GRAVITY						0.009f

#define SKULLS_BBOX_WIDTH					18
#define SKULLS_BBOX_HEIGHT					21

#define SKULLS_BBOX_HEIGHT_DIE				9

#define SKULLS_STATE_FLYRIGHT				100
#define SKULLS_STATE_ATTACK					200
#define SKULLS_STATE_DIE					300
	
#define SKULLS_ANI_FLYRIGHT					0
#define SKULLS_ANI_ATTACK					1
#define SKULLS_ANI_DIE						2

#define SKULLS_MAXHEALTH					2


#define SKULLS_TIME_DELAYCHANGESTATE		1500
#define SKULLS_TIME_DELAYSHOT				1500

class Skulls: public Enemy
{
	vector<BulletSkulls*> bullet;
	bool isfly;
	bool directY;
	
	Timer* StateDelayTimer = new Timer(SKULLS_TIME_DELAYCHANGESTATE);
	Timer* DelayShot = new Timer(SKULLS_TIME_DELAYSHOT);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	virtual void Render();


public:
	Skulls(float x, float y, LPGAMEENTITY t);
	virtual int randomdirectY();
	virtual bool CheckRun();
	virtual void Move();
	virtual void Attack();
	virtual void SetState(int state);
	virtual void Activation();
};

