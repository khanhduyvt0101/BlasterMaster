#include "Textures.h"
#include "Game.h"

CTextures * CTextures::__instance = NULL;

CTextures * CTextures::GetInstance()
{
	if (__instance == NULL) __instance = new CTextures();
	return __instance;
}


CTextures::CTextures()
{
}

void CTextures::Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor)
{
	textures[id] = CGame::GetInstance()->LoadTexture(filePath);
}

LPDIRECT3DTEXTURE9 CTextures::Get(unsigned int i)
{
	return textures[i];
}

