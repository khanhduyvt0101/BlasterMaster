#include "Bullet.h"
Bullet::Bullet()
{
	isDone = true;
}

Bullet::~Bullet()
{
}

void Bullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void Bullet::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	Entity::Update(dt, coObjects);
}

void Bullet::Render()
{
}
