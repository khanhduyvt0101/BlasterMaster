#pragma once
#include "Enemy.h"
#include "Timer.h"
#include <ctime>
#include "BulletFloaters.h"


#define FLOATERS_SPEED_CONSTVX							0.05f
#define FLOATERS_SPEED_MAXVY							1.2f



#define FLOATERS_BBOX_WIDTH								18
#define FLOATERS_BBOX_HEIGHT							15
#define FLOATERS_BBOX_HEIGHT_DIE						9

#define FLOATERS_STATE_FLY								100
#define FLOATERS_STATE_ATTACK							200
#define FLOATERS_STATE_DIE								300

#define FLOATERS_ANI_FLY								0
#define FLOATERS_ANI_ATTACK								1
#define FLOATERS_ANI_DIE								2

#define FLOATERS_SITEACTIVE_PLAYER						100

#define FLOATERS_MAXTIME_ATTACK							8000

#define FLOATERS_MAXHEALTH								1

#define FLOATERS_TIME_DELAYANI							350


class Floaters : public Enemy
{
	vector<BulletFloaters*> bullet;
	
	Timer* delayTimer = new Timer(randomTimeAttack());
	Timer* delayTimeranishot = new Timer(FLOATERS_TIME_DELAYANI);
	float vxR, vyR;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	virtual void Render();

public:
	Floaters(float x, float y, LPGAMEENTITY t);
	void Attack();
	void randomSpeed();
	int randomTimeAttack();
	virtual void SetState(int state);
	virtual void Activation();


};