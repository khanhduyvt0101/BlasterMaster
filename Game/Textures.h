#pragma once
#include "Game.h"
#include <unordered_map>
#include <d3dx9.h>

using namespace std;

/*
	Manage texture database
*/
class CTextures;
//typedef CTextures* LPTEXTURE;
class CTextures
{
	static CTextures* __instance;

	unordered_map<int, LPDIRECT3DTEXTURE9> textures;

public:
	CTextures();
	void Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor);
	LPDIRECT3DTEXTURE9 Get(unsigned int i);

	void Clear();
	void ClearAt(unsigned int i);
	static CTextures* GetInstance();
};