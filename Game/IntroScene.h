#pragma once
#include "Entity.h"

class IntroScene : public Entity
{
	static IntroScene* instance;
public:
	int typeScene;
	IntroScene(float x = 0.0f, float y = 0.0f);
	static IntroScene* GetInstance();

	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL);
	virtual void Render();


	void SetType(int type) { typeScene = type; };
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};