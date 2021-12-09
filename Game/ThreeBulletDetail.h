#pragma once
#include "Bullet.h"
#include <math.h>

#define BULLET_SPEED					0.25f


#define MINI_BULLET_BBOX_WIDTH			4
#define MINI_BULLET_BBOX_HEIGHT			4

#define THREE_BULLET_BBOX_WIDTH			19
#define THREE_BULLET_BBOX_HEIGHT		7

#define BIG_BULLET_BBOX_WIDTH			22
#define BIG_BULLET_BBOX_HEIGHT			6

#define SMALL_BULLET_JASON_ANI_RIGHT	0
#define SMALL_BULLET_JASON_ANI_TOP		1

#define BIG_BULLET_JASON_ANI_RIGHT		2
#define BIG_BULLET_JASON_ANI_TOP		3

#define BULLET_JASON_BANG_ANI			4
#define MINI_BULLET_ANI					5

#define ANIMATION_SET_JASON_BULLET		4
#define SMALL_JASON_BULLET_DELAY		560

class ThreeBulletDetail : public Bullet
{
public:
	ThreeBulletDetail();
	~ThreeBulletDetail();
	int isThreeBullet = 0;
	bool isCountBack = true;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL);
	void Render();
};

