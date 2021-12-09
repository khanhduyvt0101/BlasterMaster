
#pragma once
#include "Enemy.h"

#define ORBS_GRAVITY					0.0015f
#define ORBS_ATTACK_SPEED				0.8f

#define ORBS_BBOX_WIDTH					18
#define ORBS_BBOX_HEIGHT				18

#define ORBS_STATE_ATTACK				100
#define ORBS_STATE_DIE					200

#define ORBS_ANI_ATTACK					0
#define ORBS_ANI_DIE					1

#define ORBS_SITEATTACK_TOY				20

#define ORBS_MAXHEALTH					4

#define ORBS_SITEACTIVE_PLAYER			100

class Orbs: public Enemy
{

	bool isColliTarget;
	
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	virtual void Render();


public:
	Orbs(float x, float y, LPGAMEENTITY t);
	virtual void SetState(int state);
	virtual void Activation();
	
};

