#include "Sound.h"
Sound* Sound::Instance = NULL;
Sound::Sound()
{
	Init();
	LoadResources();
}
Sound* Sound::GetInstance()
{
	if (Instance == NULL)
	{
		Instance = new Sound();
	}
	return Instance;
}

void Sound::Init()
{
	DSManager = new CSoundManager();
	DSManager->Initialize(Game::GetInstance()->GetWindowHandle(), DSSCL_PRIORITY);
	DSManager->SetPrimaryBufferFormat(2, 22050, 16);
}


void Sound::AddSound(int type, LPTSTR filepath)
{
	CSound* Sound;
	HRESULT result = DSManager->Create(&Sound, filepath);
	if (result != DS_OK) {
		DebugOut(L"Load sound fail\n");
		return;
	}
	else
		DebugOut(L"Load sound ok\n");
	LstSound[type] = Sound;
}

void Sound::Play(GSOUND Type, bool Replay)
{
	LstSound[Type]->Play(0, Replay, 0L);
}

void Sound::LoadResources()
{
	ifstream ifs("sound/LSounds.txt", ios::in);
	int nS;
	ifs >> nS;
	for (int i = 0; i < nS; i++)
	{
		int Type;
		string fname;
		ifs >> Type;
		ifs >> fname;
		std::string stdstr(fname);
		CString Str(stdstr.c_str());
		LPTSTR lptszStr = Str.GetBuffer(0);
		Str.ReleaseBuffer();
		AddSound(Type, lptszStr);
	}
}
