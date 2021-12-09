#include "HealthBar.h"

HealthBar::HealthBar(int maxHealth, bool isGun)
{
	currentOwnerHealth = maxHealth;
	maxOwnerHealth = maxHealth;
	this->isGun= isGun;

	for (int i = 0; i < currentOwnerHealth; i++)
	{
		LPANIMATION_SET aniSet = CAnimationSets::GetInstance()->Get(ANIMATION_SET_PLAYERHP);
		health.push_back(aniSet);
	}
}

HealthBar::~HealthBar() {}

void HealthBar::Update(int currentHealth, float X, float Y)
{
	currentOwnerHealth = currentHealth;
	if (currentOwnerHealth <= 0)
		return;
	x = X;
	y = Y;
}

void HealthBar::Render()
{
	if (currentOwnerHealth <= 0)
		return;
	else
	{
		for (int j = 0; j < currentOwnerHealth; j++)
		{
			if (isGun)
			{
				health[j]->at(HEALTH_TYPE_GUN_UNIT)->Render(1, x, y + 57 - j * HEALTH_SPACE_UNIT);
			}
			else
			{
				health[j]->at(HEALTH_TYPE_PLAYER_UNIT)->Render(1, x, y + 57 - j * HEALTH_SPACE_UNIT);
			}
		}
	/*	for (int l = currentOwnerHealth; l < maxOwnerHealth; l++)
		{
			health[l]->at(HEALTH_TYPE_PLAYER_LOST)->Render(-1, x, y + l * HEALTH_SPACE_UNIT);
		}*/
	}
}
