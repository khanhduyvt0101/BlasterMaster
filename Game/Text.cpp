#include "Text.h"

Text::Text() {}

Text::~Text() {}

void Text::Render(float posX, float posY, const string& str)
{
	for (UINT i = 0; i < str.size(); i++)
	{
		//Hien tai chi dung so'
		if (str[i] >= '0' && str[i] <= '9')
		{
			textSpr = CSprites::GetInstance()->Get(str[i] - '0' + TEXT_SPRITES);
		}
		else	//va dau tru	
		{
			textSpr = CSprites::GetInstance()->Get(TEXT_SUBSIGN_SPRITES);
		}
		textSpr->Draw(-1, posX + i * TEXT_SPACE_UNIT, posY);
	}
}

string Text::FillZeroString(string str, UINT MaxStringLenght)
{
	while (str.size() < MaxStringLenght)
		str = "0" + str;
	return str;
}
