#pragma once
#pragma once
#include "Brick.h"
#define ID_ANI_GOLDEN_BRICK_NORMAL 7001
#define ID_ANI_GOLDEN_BRICK_GOLD 7002
#define GOLDEN_BRICK_STATE_NORMAL 0
#define GOLDEN_BRICK_STATE_MOVE_UP 1
#define GOLDEN_BRICK_STATE_MOVE_DOWN 2
#define GOLDEN_BRICK_STATE_GOLD 3
#define GOLDEN_BRICK_STATE_GOLD_TIMEOUT 12000
#define GOLDEN_BRICK_MOVE_SPEED 0.1f
class CGoldenBrick :
	public CBrick
{
protected:
	float minY, maxY;
	ULONGLONG state_start;
public:
	CGoldenBrick(float x, float y) : CBrick(x, y)
	{
		minY = y - BRICK_WIDTH / 2;
		maxY = y;
		SetState(GOLDEN_BRICK_STATE_NORMAL);
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking();
	virtual void HitByMario();
	virtual void Break();
	virtual void SetState(int _state);
};

