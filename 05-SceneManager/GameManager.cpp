#include "GameManager.h"
#include "ScoreEffect.h"
#include "DamageEffect.h"
#include "DataManager.h"
#include "PlayHUD.h"
#include "BrokenBrickEffect.h"
//#include "TextEffect.h"
#define ID_ANI_SCORE_100 70100
#define ID_ANI_SCORE_200 70200
#define ID_ANI_SCORE_400 70400
#define ID_ANI_SCORE_800 70800
#define ID_ANI_SCORE_1000 71000
#define ID_ANI_SCORE_2000 72000
#define ID_ANI_SCORE_4000 74000
#define ID_ANI_SCORE_8000 78000
#define ID_ANI_ONEUP_EFFECT 70001

#define DOUBLE_SCORE_TIME_OUT 2000
CGameManager* CGameManager::instance = nullptr;
CGameManager* CGameManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new CGameManager();
	}
	return instance;
}

void CGameManager::AddScoreEffect(float x, float y, int value)
{
	objects.push_back(new CScoreEffect(x, y, GetIdAniScore(value)));
	AddScore(value);
}

void CGameManager::AddCoin(int value)
{
	CDataManager::GetInstance()->AddCoin(value);
	CPlayHUD::GetInstance()->SetCoin(CDataManager::GetInstance()->GetCoin());

}

void CGameManager::AddScore(int value)
{
	CDataManager::GetInstance()->AddScore(value);
	CPlayHUD::GetInstance()->SetCoin(CDataManager::GetInstance()->GetScore());
}

void CGameManager::AddOneUpEffect(float x, float y)
{
	objects.push_back(new CScoreEffect(x, y, ID_ANI_ONEUP_EFFECT));
}

void CGameManager::AddDamageEffect(float x, float y)
{
	objects.push_back(new CDamageEffect(x, y));
}

void CGameManager::AddBrokenEffect(float x, float y)
{
	objects.push_back(new CBrokenBrickEffect(x, y));
}

void CGameManager::StomKoopa(float x, float y)
{
	ULONGLONG deltaTime = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetDeltaTime(lastTimeStomKoopa);
	if (deltaTime < DOUBLE_SCORE_TIME_OUT) {
		lastScoreKoopa *= 2;
	}
	else {
		lastScoreKoopa = 100;
	}
	switch (lastScoreKoopa)
	{
	case 16000:
		lastScoreKoopa = 8000;
		AddOneUpEffect(x, y);
		break;
	case 1600:
		lastScoreKoopa = 1000;
	default:
		AddScoreEffect(x, y, lastScoreKoopa);
		break;
	}
	objects.push_back(new CScoreEffect(x, y, GetIdAniScore(lastScoreKoopa)));
	lastTimeStomKoopa = GetTickCount64();
}

void CGameManager::StomGoomba(float x, float y)
{
	ULONGLONG deltaTime = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetDeltaTime(lastTimeStomGoomba);
	if (deltaTime < DOUBLE_SCORE_TIME_OUT) {
		DebugOut(L"Stom Goomba: %d\n", lastScoreGoomba);
		lastScoreGoomba *= 2;
	}
	else {
		lastScoreGoomba = 100;
	}
	objects.push_back(new CScoreEffect(x, y, GetIdAniScore(lastScoreGoomba)));
	lastTimeStomGoomba = GetTickCount64();
}

void CGameManager::Update(DWORD dt)
{
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->Update(dt, NULL);
	}
	PurgeDeletedObjects();

}

void CGameManager::Render()
{
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->Render();
	}
	if (text1) text1->Render();
	if (text2) text2->Render();
	if (card)card->Render();
}

int CGameManager::GetIdAniScore(int value)
{
	switch (value) {
	case 100:
		return ID_ANI_SCORE_100;
	case 200:
		return ID_ANI_SCORE_200;
	case 400:
		return ID_ANI_SCORE_400;
	case 800:
		return ID_ANI_SCORE_800;
	case 1000:
		return ID_ANI_SCORE_1000;
	case 2000:
		return ID_ANI_SCORE_2000;
	case 4000:
		return ID_ANI_SCORE_4000;
	case 8000:
		return ID_ANI_SCORE_8000;
	default:
		return 0;

	}
}

void CGameManager::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}
