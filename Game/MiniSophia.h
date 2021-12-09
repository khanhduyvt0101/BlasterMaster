#include "Entity.h"
#include <map>
#include "Timer.h"

#pragma once
#define ANIMATION_SET_MINI_SOPHIA	3

#define SOPHIA_GRAVITY						0.0004f
#define SOPHIA_MINI_WALKING_SPEED			0.07f 
#define SOPHIA_MINI_WALKING_ACC				0.0003f
#define SOPHIA_MINI_CRAWLING_SPEED			0.03f 
#define SOPHIA_MINI_GOSTAIR_SPEED			0.02F
#define SOPHIA_MINI_JUMP_SPEED_Y			0.15f

#define SOPHIA_ANI_MINI_IDLE_RIGHT				0
#define SOPHIA_ANI_MINI_WALKING_RIGHT			1
#define SOPHIA_ANI_MINI_IDLE_CRAWL_RIGHT		2
#define SOPHIA_ANI_MINI_WALKING_CRAWL_RIGHT		3
#define SOPHIA_ANI_MINI_DIE						4
#define SOPHIA_ANI_MINI_JUMP					5
#define SOPHIA_ANI_MINI_GOSTAIR					6
#define SOPHIA_ANI_MINI_IDLESTAIR				7

#define SOPHIA_MINI_STATE_IDLE			    0
#define SOPHIA_MINI_STATE_WALKING_RIGHT		100
#define SOPHIA_MINI_STATE_WALKING_LEFT		200
#define SOPHIA_MINI_STATE_CRAWLING_RIGHT	300
#define SOPHIA_MINI_STATE_CRAWLING_LEFT		400
#define SOPHIA_MINI_STATE_JUMP				500
#define SOPHIA_MINI_STATE_DIE				600
#define SOPHIA_MINI_STATE_IN				700
#define SOPHIA_MINI_STATE_OUT				800
#define SOPHIA_MINI_STATE_CRAWL				900
#define SOPHIA_MINI_STATE_CRAWL_STOP		1000
#define SOPHIA_MINI_STATE_UP_STAIR			1100
#define SOPHIA_MINI_STATE_DOWN_STAIR		1200
#define SOPHIA_MINI_STATE_IDLE_STAIR		1300

#define MAX_HEALTH						8


#define SOPHIA_MINI_BBOX_WIDTH				10
#define SOPHIA_MINI_BBOX_HEIGHT				16
#define SOPHIA_MINI_CRAWL_BBOX_WIDTH		16
#define SOPHIA_MINI_CRAWL_BBOX_HEIGHT		10


#define PLAYER_IMMORTAL_DURATION	1000
#define HIGHT_LEVER1 43

#define LAST_FRAME_DIE	13

class MiniSophia : public Entity
{
public:
	bool isOnStair = false;
	bool isDeath;
	bool isDoneDeath;
	static MiniSophia* instance;

	int level;
	int untouchable;
	bool isJumping = false;
	bool isGunFlipping = false;
	bool isJumpHandle;
	bool isImmortaling;
	int alpha;
	DWORD untouchable_start;

	Timer* immortalTimer = new Timer(PLAYER_IMMORTAL_DURATION);

	float start_x;			// initial position of Mario at scene
	float start_y;


	float backup_JumpY;
	bool isPressJump;
	bool isPressFlipGun;
	bool isCrawl;

public:
	MiniSophia(float x = 0.0f, float y = 0.0f);
	static MiniSophia* GetInstance();

	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL, vector<LPGAMEENTITY>* coEnemies = NULL);
	virtual void Render();

	//Immortal
	bool IsImmortaling() { return isImmortaling; }
	void SetImmortaling(bool immo) { isImmortaling = immo; }
	void StartImmortalingTimer() { immortalTimer->Start(); }

	void SetDirection(int d) { direction = d; }
	void SetState(int state);
	void SetPressSpace(bool isPress) { isPressJump = isPress; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void GetPositionCenter(float& x, float& y) { x = this->x + SOPHIA_MINI_BBOX_WIDTH / 2; y = this->y + SOPHIA_MINI_BBOX_HEIGHT / 2; }
	bool GetIsCrawl() { return isCrawl; }
	void SetIsCrawl(bool crawl) { isCrawl = crawl; }
	void Setvx(float vx) { vx = vx; }
	void Setvy(float vy) { vy = vy; }
	float GetDy() { return dy; }
	float Getvy() { return vy; }
	void Reset();
	void GetInfoForBullet(int& direct, float& playerx, float& playery) { direct = direction; playerx = x; playery = y; }

	//Bullet* GetPlayerMainBullet() { return mainBullet; }
	void SetInjured(int dame);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

