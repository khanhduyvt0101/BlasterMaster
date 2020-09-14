#include "GameObject.h"
#include "debug.h"
#include "Game.h"
#include "GameObject.h"

CGameObject::CGameObject(LPCWSTR texturePath)
{
	x = y = 0;
	texture = CGame::GetInstance()->LoadTexture(texturePath);
}

void CGameObject::Update(DWORD dt)
{
	
}

void CGameObject::Render()
{
	CGame::GetInstance()->Draw(x, y, texture);
}

CGameObject::~CGameObject()
{
	if (texture != NULL) texture->Release();
}

void CSophia::Update(DWORD dt)
{
	x += 0.1f*dt;
	if (x > 320) x = 0;
}
