#pragma once
#include "GameObject.h"
#include "PlayScene.h"
#define DAMAGE_EFFECT_TIMEOUT 150
#define ID_ANI_DAMAGE_EFFECT 80000
class CDamageEffect :
	public CGameObject
{
	ULONGLONG startTime = 0;
public:
	CDamageEffect(float x, float y) : CGameObject(x, y)
	{
		startTime = GetTickCount64();
	}
	void Render() override
	{
		CAnimations::GetInstance()->Get(ID_ANI_DAMAGE_EFFECT)->Render(x, y);
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override
	{
		ULONGLONG deltaTime = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetDeltaTime(startTime);
		if (deltaTime > DAMAGE_EFFECT_TIMEOUT) Delete();
	}
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	}
};

