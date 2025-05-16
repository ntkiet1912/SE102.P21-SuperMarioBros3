#pragma once

class CDataManager {
	static CDataManager* __instance;
	int coin;
	int score;
public:
	CDataManager() : coin(0), score(0) {}
	static CDataManager* GetInstance();

	void AddCoin(int value);
	void AddScore(int value);

	int GetCoin() { return coin; }	
	int GetScore() { return score; }

	void SaveData();
	void LoadData();
};