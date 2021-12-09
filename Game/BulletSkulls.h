
#pragma once
#include "BulletEnemy.h"
//#include "Player.h"
#include "Brick.h"
#include "Timer.h"
#include <ctime>

#define BULLET_SPEED						0.05f;

#define BULLET_SPEED_VY						0.3f
#define BULLET_GRAVYTY_VY					0.02f
#define BULLET_FRICTION_VX					0.0007f

#define BULLET_BBOX_WIDTH					10
#define BULLET_BBOX_HEIGHT					10

#define BULLET_SKULLS_ANI_FLY				10500
#define BULLET_SKULLS_ANI_BANG				10100

#define SKULLS_BULLET_SPEED					2

#define SKULLS_BULLET_GRAVITY				0.001f

#define SKULLS_BULLET_VFLY					0.4f

#define SKULLS_BULLET_DELAY					350

#define SKULLS_BULLET_DAMAGE				1

#define SKULLS_TIME_BULLETBANG				1800


class BulletSkulls: public BulletEnemy
{
	LPANIMATION aniBullet;
	bool isBang;
	
	Timer* TimeBang = new Timer(SKULLS_TIME_BULLETBANG);
public:
	bool _isFinish;
	BulletSkulls(float x, float y, float vx);
	void Render();
	void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetDamage();
	bool IsStart();
};

