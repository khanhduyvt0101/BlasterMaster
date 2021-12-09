#pragma once
#include "BulletEnemy.h"
#include "Player.h"
#include "Brick.h"
#include "Player.h"
#include "PlayerV2.h"

#define BULLET_SPEED					0.05f;


#define BULLET_BBOX_WIDTH				9
#define BULLET_BBOX_HEIGHT				9

#define BULLET_BOSS_ANI_FLY				2305


#define BOSS_BULLET_SPEED			2

#define BOSS_BULLET_GRAVITY			0.001f

#define BOSS_BULLET_VFLY				0.4f

#define BOSS_BULLET_DELAY			350

#define BOSS_BULLET_DAMAGE			1

class BulletBoss: public BulletEnemy
{
	bool isStart;

	LPGAMEENTITY playerE = Player::GetInstance();

	LPANIMATION aniBullet;
	float RenderVx;
	float RenderVy;
public:

	bool _isFinish;

	BulletBoss(float _x, float _y, float _posRight, float _posBottom, float _postargetLeft, float _postargetTop, float _postargetRight, float _postargetBottom);

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
	virtual void SetCenterBoundingBox(float& x, float& y, float _posLeft, float _posTop, float _posRight, float _posBottom);
};

