#include "LoadHUD.h"

void CLoadHUD::Draw()
{
	if (sprite != nullptr)
	{
		sprite->DrawOnScreen(x, y);
	}
}