#pragma once
#include "Enemy.h"
#include <ctime>
#include "Timer.h"
#include "Camera.h"
#include "BossHand.h"
#include "BulletBoss.h"

#define BOSS_SPEED_VX									0.017f
#define BOSS_SPEED_VY									0.017f

#define BOSS_BBOX_WIDTH									60
#define BOSS_BBOX_HEIGHT								65
#define BOSS_BBOX_HEIGHT_DIE							9

#define BOSS_STATE_FLY									100
#define BOSS_STATE_ATTACK								200
#define BOSS_STATE_DIE									300

#define BOSS_ANI_FLY									0
#define BOSS_ANI_ATTACK									1
#define BOSS_ANI_DIE									2

#define BOSS_MAXHEALTH									2

#define BOSS_MAXTIME_ATTACK							8000

#define BOSS_TIME_DELAYANI							350

class Boss: public Enemy
{
public:
	float tempX, tempY;

	bool isDie;

	Timer* TimeDelayX = new Timer(300);
	Timer* TimeDelayY = new Timer(300);

	Timer* TimeDie = new Timer(4000);

	Timer* DelayTime =new Timer(randomTimeAttack());
	Timer* delayTimeranishot = new Timer(BOSS_TIME_DELAYANI);

	RECT PosBoss;
	vector<BossHand*> ListHand;
	vector<BulletBoss*> bullet;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	virtual void Render();

public:
	Boss(float x, float y, LPGAMEENTITY t);
	virtual void SetState(int state);
	virtual void Activation();
	virtual void Attack();
	int randomTimeAttack();

};

