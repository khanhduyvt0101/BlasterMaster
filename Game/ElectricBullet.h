#pragma once
#include "Bullet.h"
#include <math.h>
#define ANIMATION_SET_ELECTRIC_BULLET		5	

#define ELECTRIC_BULLET_ANI_JASON			0

#define ELECTRIC_BULLET_JASON_BBOX_WIDTH	14
#define ELECTRIC_BULLET_JASON_BBOX_HEIGHT	16

#define ELECTRIC_DELAY	200
class ElectricBullet : public Bullet
{
public:
	ElectricBullet();
	~ElectricBullet();
	bool isCountBack = true;
	//void Fire(int direc, int isTargetTop, float x, float y) { x = x; y = y; };
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL);
	void Render();
};


