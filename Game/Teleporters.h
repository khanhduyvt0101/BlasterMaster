#pragma once
#include "Enemy.h"
#include "Timer.h"
#include <ctime>
#include "Camera.h"
#include "define.h"
#include "BulletEyeballs.h"

//Teleporters sử dụng class đạn của EyeBalls.

#define TELEPORTERS_BBOX_WIDTH							24
#define TELEPORTERS_BBOX_HEIGHT							32
#define TELEPORTERS_BBOX_HEIGHT_DIE						9

#define TELEPORTERS_STATE_TELE							100
#define TELEPORTERS_STATE_FREEZE						200
#define TELEPORTERS_STATE_DIE							300

#define TELEPORTERS_ANI_TELE							0
#define TELEPORTERS_ANI_FREEZE							1
#define TELEPORTERS_ANI_DIE								2

#define TELEPORTERS_TIME_DELAY							250
#define TELEPORTERS_TIME_FREEZE							1500
#define TELEPORTERS_TIME_DELAYSHOT						600

#define TELEPORTERS_SITEACTIVE_PLAYER					100

#define EYEBALLS_MAXHEALTH								1

class Teleporters : public Enemy
{
	LPANIMATION aniBullet;
	vector<BulletEyeballs*> bullet;

	bool isFreeze;

	Timer* TimeDelay = new Timer(TELEPORTERS_TIME_DELAY);
	Timer* TimeFreeze = new Timer(TELEPORTERS_TIME_FREEZE);
	Timer* TimeDelayShot = new Timer(TELEPORTERS_TIME_DELAYSHOT);

	float RenderVx;
	float RenderVy;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	virtual void Render();

public:
	Teleporters(float x, float y, LPGAMEENTITY t);
	virtual void SetState(int state);
	virtual void Activation();
	virtual void RandomPosition();
	void Attack();
};

