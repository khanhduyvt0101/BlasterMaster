#pragma once
#include "Enemy.h"
#include "Brick.h"
#include "Player.h"
#include "Timer.h"
#include <ctime>

// Chuyển động theo chiều X, gặp tường xoay theo 1 hướng.
// Hết 1 khoảng thời gian thì sẽ xoay 1 vòng và nhích random lên trên hay nhích xuống dưới 1 miếng, khi xoay 1 vòng thì random đổi chiều xoay khi dụng tường
// Ani xoay 1 vòng là xoay bên đối vs bên quay đụng tường


#define ORBEZ_WALKING_SPEED				0.05f
#define ORBEZ_JUMP_SPEED_Y				0.4f
#define ORBEZ_GRAVITY					0.006f

#define ORBEZ_BBOX_WIDTH				18
#define ORBEZ_BBOX_HEIGHT				18

#define ORBEZ_BBOX_HEIGHT_DIE			9


#define ORBEZ_STATE_FLY_RIGHT			100
#define ORBEZ_STATE_COLLISION_FLY_RIGHT_INFRONTOF	200
#define ORBEZ_STATE_COLLISION_FLY_RIGHT_BEHIND		300
#define ORBEZ_STATE_FLY_ROTATE_RIGHT360				400
#define ORBEZ_STATE_DIE					500

#define ORBEZ_ANI_FLY_RIGHT				0
#define ORBEZ_ANI_COLLISION_FLY_RIGHT_INFRONTOF		1
#define ORBEZ_ANI_COLLISION_FLY_RIGHT_BEHIND		2
#define ORBEZ_ANI_FLY_ROTATE_RIGHT360				3
#define ORBEZ_ANI_DIE					4

#define ORBEZ_TIME_ROTATION				4000

#define ORBEZ_MAXHEALTH					4

#define ORBEZ_SITEACTIVE_PLAYER			100


class OrbEz: public Enemy
{
	int ani;

	Timer* TimerRotation = new Timer(ORBEZ_TIME_ROTATION);
	bool isRotation;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	virtual void Render();


public:
	OrbEz(float x, float y, LPGAMEENTITY t);
	virtual void SetState(int state);
	virtual void Activation();
};



