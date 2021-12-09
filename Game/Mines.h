#pragma once
#include "Enemy.h"
#include "Brick.h"
#include "Player.h"
#include "BulletMines.h"
// Lúc xuất hiện thì sẽ trong trạng thái nhấp nháy. Khi chạm vài player hoặc đạn thì sẽ bay ra 2 hoặc 3 viên đạn theo các hướng khác nhau

#define MINES_GRAVITY				0.1f

#define MINES_BBOX_WIDTH			16
#define MINES_BBOX_HEIGHT			9  // Độ cao thực = 9
#define MINES_BBOX_HEIGHT_DIE		9

#define MINES_STATE_READY			100
#define MINES_STATE_DIE				200

#define MINES_ANI_READY				0
#define MINES_ANI_DIE				1

#define MINES_MAXHEALTH				1


class Mines: public Enemy
{
	vector<BulletMines*> bullet;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	virtual void Render();
	bool CheckColli;
	
public:

	Mines(float x, float y, LPGAMEENTITY t);
	virtual void SetState(int state);
	virtual void Activation();
	virtual void Attack();
	int RandomAttack();

};

