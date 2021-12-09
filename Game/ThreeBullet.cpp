#include "ThreeBullet.h"

ThreeBullet::ThreeBullet()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_THREE_BULLET));
	x = 0;
	y = 0;
	alpha = 0;
	isCollisionEnemies = 0;
	isDone = true;
	damage = 2;
	timeDelayed = 0;
	timeDelayMax = THREE_BULLET_DELAY;
	bullet_top = new ThreeBulletDetail();
	bullet_mid = new ThreeBulletDetail();
	bullet_bot = new ThreeBulletDetail();
	damage = 2;
}

ThreeBullet::~ThreeBullet() {}

void ThreeBullet::Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects)
{
	timeDelayed += dt;
	if (timeDelayed > timeDelayMax)
		isDone = true;
	bullet_top->Update(dt, colliable_objects);
	bullet_mid->Update(dt, colliable_objects);
	bullet_bot->Update(dt, colliable_objects);
}

void ThreeBullet::Render()
{
	bullet_top->Render();
	bullet_mid->Render();
	bullet_bot->Render();
}

void ThreeBullet::OffBoundingBox(int n)
{
	bullet_top->SetBBARGB(n);
	bullet_mid->SetBBARGB(n);
	bullet_bot->SetBBARGB(n);
}

void ThreeBullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}

void ThreeBullet::FireThreeBullet(int direction, float posX, float posY)
{
	sound->Reset(GSOUND::S_BULLET_SOPHIA);
	sound->Play(GSOUND::S_BULLET_SOPHIA, false);
	timeDelayed = 0;
	isDone = false;
	bullet_top->isThreeBullet = 1;
	bullet_mid->isThreeBullet = 2;
	bullet_bot->isThreeBullet = 3;
	bullet_top->Fire(1, direction, 0, posX, posY);
	bullet_mid->Fire(1, direction, 0, posX, posY);
	bullet_bot->Fire(1, direction, 0, posX, posY);
}