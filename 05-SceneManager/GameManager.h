#pragma once
#include <vector>
#include "GameObject.h"
using namespace std;
class CGameManager
{
	static CGameManager* instance;
	vector<CGameObject*> objects;
	ULONGLONG lastTimeStomKoopa = -1;
	ULONGLONG lastTimeStomGoomba = -1;
	int lastScoreKoopa = 100;
	int lastScoreGoomba = 100;
	int idCard;
	CGameObject* text1 = NULL;
	CGameObject* text2 = NULL;
	CGameObject* card = NULL;
	float winTime = -1;

public:
	static CGameManager* GetInstance();
	void AddScoreEffect(float x, float y, int value);
	void AddCoin(int value);
	void AddScore(int value);
	void AddOneUpEffect(float x, float y);
	void AddDamageEffect(float x, float y);
	void AddBrokenEffect(float x, float y);
	void AddGoalCardEffect(float x, float y);
	void StomKoopa(float x, float y);
	void StomGoomba(float x, float y);
	void Update(DWORD dt);
	void Render();
	int GetIdAniScore(int value);
	void PurgeDeletedObjects();
};

