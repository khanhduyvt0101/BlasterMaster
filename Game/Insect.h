
#pragma once
#include "Enemy.h"
#include "Timer.h"

#define INSECT_FLYUP_SPEED			0.039f
#define INSECT_FLYDOWN_SPEED		0.01f
#define INSECT_FLYLR_SPEED			0.008f

#define INSECT_GRAVITY				0.2f

#define INSECT_BBOX_WIDTH			18
#define INSECT_BBOX_HEIGHT			18
#define INSECT_BBOX_HEIGHT_DIE		9

#define INSECT_STATE_FLY			100
#define INSECT_STATE_DOWN			200
#define INSECT_STATE_DIE			300

#define INSECT_ANI_FLY				0
#define INSECT_ANI_DOWM				1
#define INSECT_ANI_DIE				2

#define INSECT_DELAYTIME_FLYUP		500
#define INSECT_DELAYTIME_FLYDOWM	1400

#define INSECT_MAXHEALTH			1

#define INSECT_SITEACTIVE_PLAYER	100

class Insect: public Enemy
{
	int isFly;
	Timer* delayTimerUp = new Timer(INSECT_DELAYTIME_FLYUP);
	Timer* delayTimerDowm = new Timer(INSECT_DELAYTIME_FLYDOWM);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	virtual void Render();


public:
	Insect(float x, float y, LPGAMEENTITY t);
	virtual void SetState(int state);
	virtual void Activation();
};

