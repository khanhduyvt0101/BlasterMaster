#pragma once
#include "Sprites.h"
#include <string>
using namespace std;

#define TEXT_SPACE_UNIT			15
#define TEXT_SPRITES			84
#define TEXT_SUBSIGN_SPRITES	94

class Text
{
	LPSPRITE textSpr;
public:
	Text();
	~Text();
	void Render(float posX, float posY, const string& str);

	string FillZeroString(string str, UINT MaxStringLenght);
};

