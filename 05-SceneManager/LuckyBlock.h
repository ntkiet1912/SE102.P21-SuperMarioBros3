#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_LUCKY_BLOCK_NORMAL -3000
#define ID_ANI_LUCKY_BLOCK_EMPTY -3001
#define LUCKYBLOCK_WIDTH 16
#define LUCKYBLOCK_BBOX_WIDTH 16
#define LUCKYBLOCK_BBOX_HEIGHT 16

#define LUCKYBOX_VY 0.17f
#define LUCKYBLOCK_NORMAL 2000
#define LUCKYBLOCK_EMPTY 2001
#define ITEM_UPGRADELEVEL 1
#define ITEM_COIN 2
#define ITEM_1UP 3

class CLuckyBlock :
	public CGameObject
{
protected:
	float initY, maxY;
	float ay ;
	int containItemIndex;
	bool isEmpty;
	bool isHit;

	bool isReturnToOriginalPos;
	bool hasSpawned;
	bool flagForMushroom;
	void spawnItem();
public:
	CLuckyBlock(float x, float y, int containItemIndex) : CGameObject(x, y)
	{
		// avoiding error when file scene__.txt not including enough arguments
		if(containItemIndex > 0)
			this->containItemIndex = containItemIndex;
		else 
			this->containItemIndex = 2;

		initY = y;
		maxY = y - 16;
		ay = 0;

		isHit = false;
		isEmpty = false;
		isReturnToOriginalPos = false;
		hasSpawned = false;
		flagForMushroom = false;
	}

	bool getIsEmpty() { return isEmpty; }
	void setIsEmpty(bool isEmpty) { this->isEmpty = isEmpty; }
	void setIsHit(bool isHit) { this->isHit = isHit; }
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnNoCollision(DWORD dt);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

};

