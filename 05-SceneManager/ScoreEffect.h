#pragma once
#include "GameObject.h"
#include "PlayScene.h"
#define SCORE_EFFECT_TIMEOUT 500
#define SCORE_EFFECT_SPEED 0.05f
class CScoreEffect :
	public CGameObject
{
	int idAni;
	ULONGLONG startTime = 0;

public:
	CScoreEffect(float x, float y, int idAni) : CGameObject(x, y)
	{
		this->idAni = idAni;
		startTime = GetTickCount64();
	}
	void Render() override
	{
		CAnimations::GetInstance()->Get(idAni)->Render(x, y);
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override
	{
		y -= dt * SCORE_EFFECT_SPEED;
		ULONGLONG deltaTime = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetDeltaTime(startTime);
		if (deltaTime > SCORE_EFFECT_TIMEOUT) Delete();
	}
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	}
};


