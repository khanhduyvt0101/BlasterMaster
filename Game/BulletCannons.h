#pragma once
#include "Player.h"
#include "Brick.h"
#include "Player.h"
#include "BulletEnemy.h"

#define BULLET_SPEED					0.1f


#define BULLET_BBOX_WIDTH				8
#define BULLET_BBOX_HEIGHT				8

#define BULLET_CANNONS_ANI_FLY			10000

#define CANNONS_BULLET_DELAY			350

#define CANNONS_BULLET_DAMAGE			1

class BulletCannons: public BulletEnemy
{
	bool isStart;
	LPGAMEENTITY playerE = Player::GetInstance();
	LPANIMATION aniBullet;
	int direct;
	int lr;

public:
	bool _isFinish;

	BulletCannons(float _x, float _y, float _posRight, float _posBottom, int _direct, int lr);

	void Render();
	void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetDamage();
	virtual void SetCenterBoundingBox(float& x, float& y, float _posLeft, float _posTop, float _posRight, float _posBottom);
};

