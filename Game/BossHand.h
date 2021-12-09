#pragma once

#include "BulletEnemy.h"
#include "Brick.h"
#include "Player.h"
#include "Timer.h"
#include <ctime>


#define HAND_SPEED_VY_THANG				0.06f
#define HAND_SPEED_VX_VONG				0.1f
#define HAND_SPEED_VY_VONG				0.06f

#define HAND_SPEED_FOLLOW					0.001



#define HAND_BBOX_WIDTH_BONES				17
#define HAND_BBOX_HEIGHT_BONES				17
#define HAND_BBOX_WIDTH_CLAW				18
#define HAND_BBOX_HEIGHT_CLAW				34

#define STATE_1_DOWN_THANG					100
#define STATE_2_DOWN_VONG					200
#define STATE_3_UP_THANG					300
#define STATE_4_UP_VONG						400

#define HAND_BOSS_ANI_BONES				2302
#define HAND_BOSS_ANI_CLAW				2301

#define HAND_TIME_CHANGE_STATE			1500
#define HAND_TIME_DELAY					500

class BossHand: BulletEnemy
{
	LPANIMATION aniBullet;

	Timer* TimeChangeDelay = new Timer(HAND_TIME_CHANGE_STATE);
	Timer* TimeDelay = new Timer(HAND_TIME_DELAY);

	int species; //1: Bones, 2:Claw
	int direct; //-1:left, 1:right
	float RenderVx, RenderVy;


	D3DXVECTOR2 SpeedTarget;
	
public:
	float _vx;
	float _vy;
	float _xHand, _yHand, _xTarget, _yTarget;
	int flag;
	BossHand(float _x, float _y, int _species, int _direction);
	void Render();
	void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	int GetDamage();
	void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects, RECT _target, D3DXVECTOR2 Speed, D3DXVECTOR2 SpeedBoss, RECT posBoss);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
	
	void SetCenterBoundingBox(float& x, float& y, float _posLeft, float _posTop, float _posRight, float _posBottom);
	int GetSpecies();
	
	void RenderSpeedFollowTarget(float _BULLET_SPEED, float _x, float _y);

	int GetDirect();
	RECT GetPos();
	void SetSpeed(float _vx, float _vy);
	void SetPosition(float _x, float _y);


	D3DXVECTOR2 GetSpeed();
	float GetVx() { return vx; }
	float GetVy() { return vy; }
	float GetX() { return x; }
	float GetY() { return y; }

};

