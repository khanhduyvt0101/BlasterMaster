#pragma once
#include "Game.h"
#include "DSutil.h"
#include <atlstr.h>
#include <unordered_map>
#include <iostream>
#include <fstream>

using namespace std;

enum GSOUND {

	S_INTRO = 1,
	S_MAP = 2,
	S_WARNING = 3,
	S_BOSS = 4,
	S_ENDSCENE23 = 5,
	S_BULLET_SOPHIA = 6,
	S_BULLET_JASOM = 7,
	S_JUMP = 8,
	S_BULLET_EXPLODE = 9,
	S_ITEM = 10,
	S_HEALTH = 11,
	S_BOSSDEATH = 12,
};
class Sound
{
private:
	unordered_map<int, CSound*> LstSound;
	CSoundManager* DSManager;
public:
	static Sound* Instance;
	static Sound* GetInstance();
	void Init();
	Sound();
	void AddSound(int type, LPTSTR filepath);
	void Play(GSOUND Type, bool Replay);
	bool IsPlaying(GSOUND T)
	{
		return LstSound[T]->IsSoundPlaying();
	}
	void Stop(GSOUND type)
	{
		LstSound[type]->Stop();
	}
	void Reset(GSOUND type)
	{
		LstSound[type]->Reset();
	}
	void LoadResources();
	~Sound() {}
};

