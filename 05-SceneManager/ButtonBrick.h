#pragma once
#include "Brick.h"
#include "Button.h"
#define BUTTON_BRICK_STATE_NORMAL 0
#define BUTTON_BRICK_STATE_MOVE_UP 1
#define BUTTON_BRICK_STATE_MOVE_DOWN 2
#define BUTTON_BRICK_STATE_HIT 3

#define ID_ANI_BUTTON_BRICK_NORMAL 100001
#define ID_ANI_BUTTON_BRICK_HIT 100002

#define BUTTON_BRICK_MOVE_SPEED 0.1f
class CButtonBrick :
	public CBrick
{
protected:
	float minY, maxY;
	CButton* button;
public:
	CButtonBrick(float x, float y) : CBrick(x, y)
	{
		minY = y - BRICK_WIDTH / 2;
		maxY = y;
		SetState(BUTTON_BRICK_STATE_NORMAL);
		button = NULL;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	int IsBlocking() { return 1; }
	void SetState(int state);
};

