#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>

#include "Scene.h"
#include "SceneManager.h"
#include "Game.h"
#include "Camera.h"

#include "Player.h"
#include "PlayerV2.h"
#include "MiniSophia.h"
#include "HUD.h"
#include "Brick.h"
#include "Gate.h"
#include "PowerUp.h"
#include "GunUp.h"

#include "Centipede.h"
#include "Golem.h"
#include "Domes.h"
#include "Floaters.h"
#include "Insect.h"
#include "Orbs.h"
#include "Skulls.h"
#include "OrbEz.h"
#include "Mines.h"
#include "Eyeballs.h"
#include "Teleporters.h"
#include "Cannons.h"
#include "Boss.h"

#include "LavaBrick.h"
#include "BrickBreaker.h"

#include "MainJasonBullet.h"
#include "ElectricBullet.h"
#include "BigSophiaBullet.h"
#include "ThreeBullet.h"

#include "BulletEnemy.h"
#include <iostream>
#include <fstream>

#include "Entity.h"
#include "IntroScene.h"
#include "Grid.h"

using namespace std;

class PlayScene : public Scene
{
private:
	Grid* grid;
public:

	bool checkCamMove = false; //kiem tra luot cap, fix bug qua cong map 2
	int typeScene = 1;
	float oldPosX;
	float oldPosY;
	int directMoveCam = -1;
	float posX, posY;
	int typeSophia;
	float nCamXGo;
	float nCamXBack;
	float nCamYGo;
	float nCamYBack;
	float camMap1X;
	float camMap1Y;
	bool tempNeed;
	bool isTouchStair;
	DWORD timeResetCam;

	bool isWarning = false;//den hoi chuyen lai thanh false
	bool isBoss = false;
	bool isDark = true; //den hoi chuyen lai thanh false
	bool isLight = false; 
	float colorSubtrahend = 0; // muc do tang dan do sang
	float alpha = 0; // do sang man hinh warning
	int counting = 0;
	bool isBossDeath = false;
	bool isBossSpawn = false;
	bool isCheckWarning = false;
	//void SetIsMiniSophia() { type = 0; }
	//bool isMiniSophia


protected:
	Boss* boss;
	Player* player;
	PlayerV2* playerV2;
	MiniSophia* sophia;
	HUD* gameHUD;
#pragma region Bullet
	Bullet* bullet1;
	Bullet* bullet2;
	Bullet* bullet3;
	Bullet* supBullet;
	Bullet* bigBullet1;
	Bullet* bigBullet2;
	Bullet* bigBullet3;
	IntroScene* introScene;
	ThreeBullet* supBulletThree;
#pragma endregion
	PowerUp* powerUp;
	GunUp* gunUp;
	vector<LPGAMEENTITY> listGates;
	vector<LPGAMEITEM> listItems;
	vector<LPGAMEENTITY> listStairs;
	vector<LPGAMEENTITY> listObjects;
	vector<LPGAMEENTITY> listEnemies;
	vector<LPGAMEENTITY> listEnemiesForBullet;
	vector<LPGAMEENTITY> totalObjectsIntoGrid;
	vector<LPGAMEENTITY> listObjectLoad;
	vector<LPBULLET> listBullets;
	vector<LPBULLET> listBigBullets;
	vector<LPGAMEENTITY> listGateImage;



	vector<LPCWSTR> listSceneFilePath;
	vector<int> listWidth;
	vector<int> listHeight;

	Camera* gameCamera;

	int idStage;
	int mapWidth, mapHeight;
	int camMaxWidth;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_CLEARTEXTURES(string line);
	void _ParseSection_CLEARSPRITES(string line);
	void _ParseSection_CLEARANIMATIONS(string line);
	void _ParseSection_CLEARANIMATION_SETS(string line);

	//Get stage objects' link and tilemap resources
	void _ParseSection_SCENEFILEPATH(string line);


public:
	PlayScene();
	void LoadBaseObjects();
	void LoadBaseTextures();
	void ChooseMap(int whatStage);
	void PlayerGotGate();
	void PlayerGotCar();
	void PlayerTouchEnemy();
	void PlayerCollideItem();
	void PlayerGotGateV2();
	void PlayerTouchStair();
	virtual void LoadSceneObjects();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void GetObjectFromGrid();
	void DarkenTheScreen();
	void DarkenTheScreenToEnd();
	Item* RandomItem(float x, float y);
	Item* DropItem(EntityType createrType, float x, float y, int idCreater = 0);

	friend class PlayScenceKeyHandler;
};


class PlayScenceKeyHandler : public ScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	PlayScenceKeyHandler(Scene* s) :ScenceKeyHandler(s) {};
};


