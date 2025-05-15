#include "PlayHUD.h"
#include <string>
#include "AssetIDs.h"
#include "DataManager.h"
CPlayHUD* CPlayHUD::__instance = nullptr;

CPlayHUD::CPlayHUD()
{
	background = new CLoadHUD(CSprites::GetInstance()->Get(ID_SPRITE_HUD_BACKGROUND), 129, 8);
	for (int i = 0; i < 7; i++)
	{
		score.push_back(new CLoadHUD(CSprites::GetInstance()->Get(ID_SPRITE_HUD_NUMBER_0), 70 + 8 * i, 12));
	}
	for (int i = 0; i < 2; i++)
	{
		coin.push_back(new CLoadHUD(CSprites::GetInstance()->Get(ID_SPRITE_HUD_NUMBER_0), 149 + 8 * i, 20));
	}
	for (int i = 0; i < 3; i++)
	{
		time.push_back(new CLoadHUD(CSprites::GetInstance()->Get(ID_SPRITE_HUD_NUMBER_0),  141 + 8 * i, 12));
	}
	CDataManager::GetInstance()->LoadData();
	SetCoin(CDataManager::GetInstance()->GetCoin());
	SetScore(CDataManager::GetInstance()->GetScore());
}

CPlayHUD* CPlayHUD::GetInstance()
{
	if (__instance == nullptr)
	{
		__instance = new CPlayHUD();
	}
	return __instance;
}

void CPlayHUD::Render()
{
	background->Draw();
	for (int i = 0; i < score.size(); i++)
	{
		score[i]->Draw();
	}
	for (int i = 0; i < coin.size(); i++)
	{
		coin[i]->Draw();
	}
	for (int i = 0; i < time.size(); i++)
	{
		time[i]->Draw();
	}
}

void CPlayHUD::SetCoin(int coinValue)
{
	string coinStr = to_string(coinValue);
	while (coinStr.length() < 2)
		coinStr = "0" + coinStr; 

	if (coinStr.length() > 2)
		coinStr = coinStr.substr(coinStr.length() - 2, 2);

	for (int i = 0; i < 2; i++)
	{
		int digit = coinStr[i] - '0';
		int spriteId = ID_SPRITE_HUD_NUMBER_0 + digit;
		coin[i]->SetSprite(CSprites::GetInstance()->Get(spriteId));
	}
}

void CPlayHUD::SetScore(int scoreValue)
{
	string scoreStr = to_string(scoreValue);
	while (scoreStr.length() < 7)
		scoreStr = "0" + scoreStr;
	if (scoreStr.length() > 7)
		scoreStr = scoreStr.substr(scoreStr.length() - 7, 7);
	for (int i = 0; i < 7; i++)
	{
		int digit = scoreStr[i] - '0';
		int spriteId = ID_SPRITE_HUD_NUMBER_0 + digit;
		score[i]->SetSprite(CSprites::GetInstance()->Get(spriteId));
	}
}

void CPlayHUD::SetTime(int timeValue)
{
	string timeStr = to_string(timeValue);
	while (timeStr.length() < 3)
		timeStr = "0" + timeStr;
	if (timeStr.length() > 3)
		timeStr = timeStr.substr(timeStr.length() - 3, 3);
	for (int i = 0; i < 3; i++)
	{
		int digit = timeStr[i] - '0';
		int spriteId = ID_SPRITE_HUD_NUMBER_0 + digit;
		time[i]->SetSprite(CSprites::GetInstance()->Get(spriteId));
	}
}