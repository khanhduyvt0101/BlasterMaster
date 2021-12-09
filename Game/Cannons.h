#pragma once
#include "Enemy.h"
#include "Timer.h"
#include <ctime>
#include "Camera.h"
#include "define.h"
#include "BulletCannons.h"

#define CANNONS_BBOX_WIDTH								26
#define CANNONS_BBOX_HEIGHT								26
#define CANNONS_BBOX_HEIGHT_DIE							9

#define CANNONS_STATE_NORMAL							100
#define CANNONS_STATE_ATTACK_HORIZONTAL					200
#define CANNONS_STATE_ATTACK_VERTICAL					300
#define CANNONS_STATE_DIE								400

#define CANNONS_ANI_NORMAL								0
#define CANNONS_ANI_ATTACK_HORIZONTAL					1
#define CANNONS_ANI_ATTACK_VERTICAL						2
#define CANNONS_ANI_DIE									3

#define CANNONS_SITEACTIVE_PLAYER						100

#define CANNONS_TIME_STATE								500

#define CANNONS_MAXHEALTH								2

class Cannons: public Enemy
{
	LPANIMATION aniBullet;
	vector<BulletCannons*> bullet;
	bool isAttack;

	int direct; // 1: Ngang, -1:Dọc

	Timer* TimeState = new Timer(CANNONS_TIME_STATE);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	virtual void Render();

public:
	Cannons(float x, float y, LPGAMEENTITY t);
	virtual void SetState(int state);
	virtual void Activation();

	void Attack();
};

