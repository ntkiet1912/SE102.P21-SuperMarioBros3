#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_LUCKY_BLOCK_NORMAL -3000
#define ID_ANI_LUCKY_BLOCK_EMPTY -3001
#define LUCKYBLOCK_WIDTH 16
#define LUCKYBLOCK_BBOX_WIDTH 14
#define LUCKYBLOCK_BBOX_HEIGHT 15

#define LUCKYBOX_VY 0.17f
#define LUCKYBLOCK_NORMAL 2000
#define LUCKYBLOCK_EMPTY 2001
#define ITEM_UPGRADELEVEL 1
class CLuckyBlock :
	public CGameObject
{
protected:
	float initY, collisionY;
	float ay ;
	int containItemIndex;
	bool isEmpty;
	ULONGLONG bouncingTime;
	bool isHit;
	void spawnItem();
public:
	CLuckyBlock(float x, float y) : CGameObject(x, y)
	{
		containItemIndex = 1;
		isEmpty = false;
		bouncingTime = -1;
		initY = y;
		collisionY = y - 16;
		isHit = false;
		state = LUCKYBLOCK_NORMAL;
		ay = 0;
	}

	bool getIsEmpty() { return isEmpty; }
	void setIsEmpty(bool isEmpty) { this->isEmpty = isEmpty; }
	void setIsHit(bool isHit) { this->isHit = isHit; }
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnNoCollision(DWORD dt);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);

};

