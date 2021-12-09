#pragma once
#include "Enemy.h"
#include "Brick.h"
#include "Player.h"
#include "Timer.h"
#include <ctime>

#define GOLEM_WALKING_SPEED				0.018f
#define GOLEM_JUMP_SPEED_Y				0.35f
#define GOLEM_GRAVITY					0.0015f

#define GOLEM_BBOX_WIDTH				14
#define GOLEM_BBOX_HEIGHT				23

#define GOLEM_BBOX_HEIGHT_DIE			9


#define GOLEM_STATE_WALKING				100
#define GOLEM_STATE_DIE					200

#define GOLEM_ANI_WALKING				0
#define GOLEM_ANI_DIE					1


#define GOLEM_TIME_FOLLOW_PLAYER_MAX	1500
#define GOLEM_TIME_DONTFOLLOW_PLAYER	1000

#define GOLEM_SITEFOLLOW_PLAYER			80
#define GOLEM_SITEACTIVE_PLAYER			100

#define GOLEM_MAXHEALTH					4



class Golem: public Enemy
{
	bool firstFollow;
	bool isFollow;
	bool afterFollow;
	DWORD timeFollow;

	Timer* readyTimer = new Timer(randomTimeJump());
	Timer* delayTimer = new Timer(GOLEM_TIME_DONTFOLLOW_PLAYER);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	virtual void Render();
	

public:
	Golem(float x, float y, LPGAMEENTITY t);
	int randomTimeJump();
	void FollowTarget(LPGAMEENTITY target);
	virtual void SetState(int state);
	virtual void Activation();
	void SelfDestroy();
};

