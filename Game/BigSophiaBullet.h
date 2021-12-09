#pragma once
#include "Bullet.h"
#include <math.h>

#define BULLET_SPEED								0.2f
#define SPEED_SUPER_BULLET							0.625f
#define DEGREE_PER_DT								60
#define BIG_SOPHIA_BULLET_BBOX_WIDTH				8
#define BIG_SOPHIA_BULLET_BBOX_HEIGHT				10

#define BIG_SOPHIA_BULLET_JASON_ANI_RIGHT_BLACK		0
#define BIG_SOPHIA_BULLET_JASON_ANI_RIGHT_COLOR		1

#define BIG_SOPHIA_BULLET_JASON_ANI_TOP_BLACK		2
#define BIG_SOPHIA_BULLET_JASON_ANI_TOP_COLOR		3

#define BULLET_JASON_BANG_ANI						4

#define ANIMATION_SET_BIG_SOPHIA_BULLET				6
#define BULLET_DELAY								450



class BigSophiaBullet : public Bullet
{
public:
	BigSophiaBullet();
	~BigSophiaBullet();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL);
	void Render();
};

