#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000
#define ID_ANI_LUCKYBLOCK_COIN 11100
#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_INIT_VY -0.35f
#define COIN_AFTER_REACHED_MAXY_VY 0.15f
#define COIN_AY 0.0024f


class CCoin : public CGameObject {
public:
	CCoin(float x, float y) : CGameObject(x, y) {}
	virtual void Render();
	virtual void Update(DWORD dt){}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};

class CCoinFromLuckyBlock : public CCoin
{
protected:
	float vy;
	float ay;
	float maxY;
	float spawnY;
	bool reachedMaxY;
public:
	CCoinFromLuckyBlock(float x, float y) : CCoin(x, y)
	{
		reachedMaxY = false;
		vy = COIN_INIT_VY;
		ay = 0;

		// luckyblock spawn - 16;
		this->spawnY = y - 16;
		// flying to a limit then dropping down 
		this->maxY = y - 100;
	}
	void Render();
	virtual int IsCollidable() { return 0; }
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt){ y += vy * dt; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

};