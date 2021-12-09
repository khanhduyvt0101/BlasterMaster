#pragma once
#include "Entity.h"

#define GATE_BBOX_WIDTH				48
#define GATE_BBOX_HEIGHT			66

class Gate : public Entity
{
	int gateSwitchSceneId;	//Gate nay se dan~ den scene nao
public:
	bool directionCam;
	float newPlayerx, newPlayery;
	int newPlayerState;
	int typePlayer;
	float camPosX;
	float camPosY;
	Gate(float x, float y, int switchId, float playerx, float playery, int playerState, bool rsCam, int type, float camX, float camY);
	~Gate();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();

	int GetIdScene() { return gateSwitchSceneId; }
};

