#pragma once
#include "GameObject.h"

#define SOPHIA_WALKING_SPEED						0.1f
#define SOPHIA_JUMP_SPEED_Y							0.35f
#define SOPHIA_GRAVITY								0.001f

#define SOPHIA_JASON_BBOX_WIDTH						26
#define SOPHIA_JASON_BBOX_HEIGHT					17

#define SOPHIA_STATE_IDLE							99


#define SOPHIA_STATE_WALKING_RIGHT					100
#define SOPHIA_STATE_WALKING_LEFT					200
#define SOPHIA_STATE_JUMP							300
#define SOPHIA_STATE_DIE							400
#define SOPHIA_STATE_DAMED							500

#define SOPHIA_ANI_IDLE_RIGHT1						0
#define SOPHIA_ANI_IDLE_RIGHT2						1
#define SOPHIA_ANI_IDLE_RIGHT3						2
#define SOPHIA_ANI_IDLE_RIGHT4						3

#define SOPHIA_ANI_WALKING_RIGHT					4
#define SOPHIA_ANI_JASON_JUMP_UP_IDLE_RIGHT			5
#define SOPHIA_ANI_JASON_JUMP_UP_WALKING_RIGHT		6
#define SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_RIGHT		7
#define SOPHIA_ANI_JASON_JUMP_DOWN_WALKING_RIGHT	8


#define SOPHIA_ANI_IDLE_LEFT1						9
#define SOPHIA_ANI_IDLE_LEFT2						10
#define SOPHIA_ANI_IDLE_LEFT3						11
#define SOPHIA_ANI_IDLE_LEFT4						12

#define SOPHIA_ANI_WALKING_LEFT						13
#define SOPHIA_ANI_JASON_JUMP_UP_IDLE_LEFT			14
#define SOPHIA_ANI_JASON_JUMP_UP_WALKING_LEFT		15
#define SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_LEFT		16
#define SOPHIA_ANI_JASON_JUMP_DOWN_WALKING_LEFT		17


#define SOPHIA_ANI_DIE								20
//#define SOPHIA_ANI_DAMED			14

#define SOPHIA_UNTOUCHABLE_TIME 1500

class CSophia : public CGameObject
{
	
	int level;
	int untouchable;
	int currentframe=0;
	DWORD untouchable_start;
	int ani;
	LPSPRITE currentframeid;

	float start_x;			// initial position of Mario at scene
	float start_y;
public:
	bool isJump = false;
	CSophia(float x = 0.0f, float y = 0.0f);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};

