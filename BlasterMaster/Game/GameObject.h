#pragma once

#include <Windows.h>
#include <d3dx9.h>

class CGameObject
{
protected:
	float x;
	float y;

	LPDIRECT3DTEXTURE9 texture;
public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }

	CGameObject(LPCWSTR texturePath);
	void Update(DWORD dt);
	void Render();
	~CGameObject();
};
typedef CGameObject * LPGAMEOBJECT;

class CSophia : public CGameObject
{
public:
	CSophia(LPCWSTR texturePath) :CGameObject(texturePath) {};
	void Update(DWORD dt);
};