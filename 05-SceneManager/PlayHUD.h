#pragma once
#include "Sprites.h"
#include "LoadHUD.h"

class CPlayHUD
{
	static CPlayHUD* __instance;
	std::vector<CLoadHUD*> coin;
	std::vector<CLoadHUD*> time;
	std::vector<CLoadHUD*> score;
	CLoadHUD* background;
public:
	CPlayHUD();
	static CPlayHUD* GetInstance();
	void Render();
	void SetCoin(int coin);
	void SetTime(int time);
	void SetScore(int score);
};