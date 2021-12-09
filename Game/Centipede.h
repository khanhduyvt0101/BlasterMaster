#pragma once
#include "Enemy.h"
#include "Player.h"
#include "Timer.h"
#include <ctime>
#define CENTIPEDE_WALKING_SPEED 0.05f;
#define CENTIPEDE_GRAVITY		0.002f

#define CENTIPEDE_BBOX_WIDTH 17
#define CENTIPEDE_BBOX_HEIGHT 8
#define CENTIPEDE_BBOX_HEIGHT_DIE 9

#define CENTIPEDE_STATE_WALKING 100
#define CENTIPEDE_STATE_DIE 200

#define CENTIPEDE_ANI_WALKING 0
#define CENTIPEDE_ANI_DIE 1

#define CENTIPEDE_SITEFOLLOW_PLAYER			100
#define CENTIPEDE_SITEACTIVE_PLAYER			135

#define CENTIPEDE_MAXHEALTH					1


class Centipede : public Enemy
{
	bool isFollow;
	LPGAMEENTITY target;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	virtual void Render();

public:
	Centipede(float x, float y, LPGAMEENTITY t);
	void FollowTarget(LPGAMEENTITY target);
	virtual void SetState(int state);
	virtual void Activation();
	void SelfDestroy();
};