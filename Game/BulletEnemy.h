#pragma once
#include "Entity.h"
#include "define.h"
#include "Enemy.h"

#define DISTANCE_TO_GUN_WIDTH   9
#define DISTANCE_TO_GUN_HEIGHT  2
#define DISTANCE_TO_BANG		4

class BulletEnemy;
typedef BulletEnemy* LPBULLETENEMY;
class BulletEnemy: public Entity
{
protected:
	EntityType BULLETENEMY;
	bool isFinish;
	DWORD lifeTime;
	bool isUsedOnceTime;
	bool isCollision;

public:
	BulletEnemy();
	~BulletEnemy();
	bool IsFinish();
	void SetFinish();
	EntityType GetType();
	D3DXVECTOR2 posBullet;
	D3DXVECTOR2 focus;

	bool IsContain(RECT rect1, RECT rect2);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects) = 0;
	virtual void Render() = 0;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Activation() {}
	virtual bool Attack(float _x, float _y, int nx);
	virtual int GetDamage() = 0;
	virtual D3DXVECTOR2 RadialMovement(D3DXVECTOR2 focus, D3DXVECTOR2 pos, float speed);
	virtual float GetDistance(D3DXVECTOR2 pos, D3DXVECTOR2 target);
	virtual D3DXVECTOR2 CreatePosFollowTarget(D3DXVECTOR2 focus, D3DXVECTOR2 pos);

};

