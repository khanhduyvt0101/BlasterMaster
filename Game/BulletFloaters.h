#pragma once
#include "BulletEnemy.h"
#include "Player.h"
#include "Brick.h"
#include "Player.h"


#define BULLET_SPEED					0.05f;


#define BULLET_BBOX_WIDTH				4
#define BULLET_BBOX_HEIGHT				4

#define BULLET_LOATERS_ANI_FLY				10000
#define BULLET_LOATERS_ANI_BANG			10100

#define LOATERS_BULLET_SPEED			2

#define LOATERS_BULLET_GRAVITY			0.001f

#define LOATERS_BULLET_VFLY				0.4f

#define LOATERS_BULLET_DELAY			350

#define LOATERS_BULLET_DAMAGE			1


class BulletFloaters: public BulletEnemy
{
	bool isStart;
	
	LPGAMEENTITY playerE = Player::GetInstance();
	
	LPANIMATION aniBullet;
	float RenderVx;
	float RenderVy;
	//float postargetLeft, postargetTop; //Left, top
	//float postargetRight, postargetBottom;
	//float posRight, posBottom;

public:
	bool _isFinish;

	BulletFloaters(float _x, float _y,float _posRight, float _posBottom, float _postargetLeft, float _postargetTop, float _postargetRight, float _postargetBottom);
		
	void Render();
	void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	void RenderSpeedFollowTarget(float _x, float _y, 
									float _posRight, 
									float _posBottom, 
									float _postargetLeft, 
									float _postargetRight, 
									float _postargetTop, 
									float _postargetBottom,
									float _BULLET_SPEED);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetDamage();
	bool IsStart();
	void Start(float _x, float _y);
	virtual void SetCenterBoundingBox(float &x, float &y,float _posLeft, float _posTop, float _posRight, float _posBottom);
};

