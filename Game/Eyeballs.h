#pragma once
#include "Enemy.h"
#include "Timer.h"
#include <ctime>
#include "Camera.h"
#include "BulletEyeballs.h"

// Chia trục tọa độ thành các phần x>0y>0 di chuyển giống x>0,y<0 (di chuyển ở dưới). X<0,y>0 di chuyển đi lên. CÒn X<0,Y<0 nữa dưới là di cchuyển ở dưới, còn nữa trên là di chuyển ở giữa.
// 3 loại:
// Loại 1: đi theo script
// Loại 2: Phóng ra từ top và left right
// Loại 3: Nhảy rồi bám vào top

#define EYEBALLS_SPEED_CONSTVX							0.05f
#define EYEBALLS_SPEED_MAXVY							1.2f
#define EYEBALLS_GRAVITY								0.002f
#define EYEBALLS_SPEED_VX								0.01f
#define EYEBALLS_SPEED_VY								0.08f

#define EYEBALLS_BBOX_WIDTH								18
#define EYEBALLS_BBOX_HEIGHT							15
#define EYEBALLS_BBOX_HEIGHT_DIE						9

#define EYEBALLS_STATE_FLY_FOLLOW						100
#define EYEBALLS_STATE_FLY_RANDOM						200
#define EYEBALLS_STATE_FLY_CLIMBTOP						300
#define EYEBALLS_STATE_ATTACK							400
#define EYEBALLS_STATE_DIE								500

#define EYEBALLS_ANI_FLY								0
#define EYEBALLS_ANI_ATTACK								1
#define EYEBALLS_ANI_DIE								2

#define EYEBALLS_SITEACTIVE_PLAYER						150

#define EYEBALLS_TIME_FLY								4000
#define EYEBALLS_TIME_DELAYFLY							2000
#define EYEBALLS_TIME_JUMP								3000
#define EYEBALLS_TIME_DELAYANI							300

#define EYEBALLS_MAXHEALTH								2

class Eyeballs :public Enemy
{


	int species;
	LPANIMATION aniBullet;
	vector<BulletEyeballs*> bullet;

	Timer* TimeFly = new Timer(EYEBALLS_TIME_FLY);
	Timer* TimeJump = new Timer(EYEBALLS_TIME_JUMP);
	Timer* delayTimeranishot = new Timer(EYEBALLS_TIME_DELAYANI);
	float RenderVx;
	float RenderVy;
	bool fly;



	float vxR, vyR;



	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	virtual void Render();

public:
	Eyeballs(float x, float y, LPGAMEENTITY t, int species);

	void RenderSpeedFollowTarget(float _BULLET_SPEED);
	D3DXVECTOR2 CreatePosFollowTarget(D3DXVECTOR2 focus, D3DXVECTOR2 pos);
	virtual void SetState(int state);
	virtual void Activation();
	void SetXYCircle();
	void randomSpeed();
	void Attack();

};

