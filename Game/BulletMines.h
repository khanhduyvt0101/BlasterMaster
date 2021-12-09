#pragma once
#include "BulletEnemy.h"
#include "Player.h"
#include "Brick.h"
#include "Player.h"
#include "Timer.h"
#include <ctime>

#define BULLET_GRAVYTY_VY					0.02f
#define BULLET_JUMP_VY						0.2f
#define BULLET_BBOX_WIDTH					4
#define BULLET_BBOX_HEIGHT					4

#define BULLET_MINES_ANI_FLY				10600
#define BULLET_MINES_ANI_BANG				10100

class BulletMines: public BulletEnemy
{
	LPANIMATION aniBullet;
	bool isBang;

public:
	bool _isFinish;
	BulletMines(float _x, float _y);
	void Render();
	void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetDamage();
	float RandomVx();
	bool IsStart();
};

