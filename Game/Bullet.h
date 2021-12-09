#pragma once
#include "Entity.h"
#include "Sound.h"
#define DISTANCE_TO_GUN_WIDTH   9
#define DISTANCE_TO_GUN_HEIGHT  2
#define DISTANCE_TO_BANG		4

class Bullet;
typedef Bullet* LPBULLET;
class Bullet : public Entity
{
public:
	int totalTime;
	float startX, startY;
	bool isDone;
	int damage;
	int isTargetTop;
	int directionY;
	int alpha;
	int isCollisionBrick;
	int isCollisionEnemies;
	float timeDelayed, timeDelayMax;
	int typeBullet;
	float oldX, oldY;
	//0 mini 1 small 2 big

public:
	Bullet();
	~Bullet();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	virtual void Render();

	void Fire(int type, 
		int direct, 
		int isGunFlip, 
		float posX, 
		float posY) { 
		sound->Reset(GSOUND::S_BULLET_SOPHIA);
		sound->Play(GSOUND::S_BULLET_SOPHIA, false);
		typeBullet = type; 
		direction = direct; 
		isTargetTop = isGunFlip; 
		x = posX + DISTANCE_TO_GUN_WIDTH; 
		y = posY + DISTANCE_TO_GUN_HEIGHT; 
		alpha = 255; isDone = false; 
		isCollisionBrick = 0; 
		isCollisionEnemies = 0; 
		oldX = posX;
		oldY = posY;
	}
	
	void BigSophiaFire(int direct, int directY, float posX, float posY, int dame) {
		direction = direct; 
		directionY = directY;
		x = posX + DISTANCE_TO_GUN_WIDTH; 
		y = posY + DISTANCE_TO_GUN_HEIGHT;
		alpha = 255; isDone = false; 
		isCollisionBrick = 0;
		isCollisionEnemies = 0; 
		damage = dame;
		startX = posX + DISTANCE_TO_GUN_WIDTH; 
		startY = posY + DISTANCE_TO_GUN_HEIGHT;
		totalTime = 0;
		oldX = posX;
		oldY = posY;
		if (directionY != 0)
			vx = 0.4;
		else
			vy = 0.4;
	}
	void ResetDelay() { timeDelayed = 0; }
	bool GetIsDone() { return isDone; }
	void SetIsDone(bool isdone) { isDone = isdone; }
	int GetDamage() { return damage; }

};