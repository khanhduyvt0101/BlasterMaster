#include "SceneManager.h"

SceneManager* SceneManager::__instance = NULL;

SceneManager* SceneManager::GetInstance()
{
	if (__instance == NULL)
		__instance = new SceneManager();
	return __instance;
}

SceneManager::SceneManager() {}

SceneManager::~SceneManager() {}

void SceneManager::Load()
{
	currentScene->LoadSceneObjects();
}
void SceneManager::Unload()
{
	currentScene->Unload();
}
void SceneManager::Update(DWORD dt)
{
	currentScene->Update(dt);
}
void SceneManager::Render()
{
	currentScene->Render();
}