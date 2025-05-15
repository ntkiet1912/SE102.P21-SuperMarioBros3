#include "DataManager.h"
#include "PlayHUD.h"
#include <fstream>

CDataManager* CDataManager::__instance = nullptr;

CDataManager* CDataManager::GetInstance()
{
	if (__instance == nullptr)
	{
		__instance = new CDataManager();
	}
	return __instance;
}

void CDataManager::AddCoin(int value)
{
	coin +=value;
	CPlayHUD::GetInstance()->SetCoin(coin);
	SaveData();
}

void CDataManager::AddScore(int value)
{
	score += value;
	CPlayHUD::GetInstance()->SetScore(score);
	SaveData();
}

void CDataManager::SaveData()
{
	std::ofstream file("data.txt");
	if (file.is_open())
	{

		file << "Score: " << score << endl;
		file << "Coin: " << coin << endl;
		file.close();
	}
}

void CDataManager::LoadData()
{
	ifstream file("data.txt");
	string key;
	int value;

	while (file >> key >> value)
	{
		if (key == "Score:")
			score = value;
		else if (key == "Coin:")
			coin = value;
	}

	file.close();
}