#pragma once
#include "Entity.h"
class Enemy: public Entity
{
protected:
	//int HP;
	bool isDamaged;
	int DamageofEnemy;
	bool isActive;
	bool isArmor;
	bool isBoss;
	int alpha;
	float RenderVx;
	float RenderVy;

	//int health;
	LPGAMEENTITY target;
	

	D3DXVECTOR2 posEnemy;
	D3DXVECTOR2 focus;

public:
	bool isDeath;
	Enemy();
	~Enemy();
	virtual void Render() = 0;
	virtual void Activation() = 0;
	virtual bool IsContain(RECT rect1, RECT rect2);
	virtual float GetDistance(D3DXVECTOR2 pos, D3DXVECTOR2 target);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* objects = NULL) = 0;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) = 0;
	virtual D3DXVECTOR2 GetPosCam();

#pragma region Hàm follow target
	virtual void SetCenterBoundingBox(float& x, float& y, float _posLeft, float _posTop, float _posRight, float _posBottom);
	D3DXVECTOR2 CreatePosFollowTarget(D3DXVECTOR2 focus, D3DXVECTOR2 pos);
	D3DXVECTOR2 RadialMovement(D3DXVECTOR2 focus, D3DXVECTOR2 pos, float speed);
	virtual void RenderSpeedFollowTarget(float _x, float _y,
		float _posRight,
		float _posBottom,
		float _postargetLeft,
		float _postargetRight,
		float _postargetTop,
		float _postargetBottom,
		float _SPEED);
#pragma endregion
	

};

