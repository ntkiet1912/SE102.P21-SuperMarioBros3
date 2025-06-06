#pragma once
#include "Brick.h"
#define BUTTON_STATE_NORMAL 0
#define BUTTON_STATE_PRESSED 1
#define ID_ANI_BUTTON 100003
#define ID_ANI_BUTTON_PRESSED 100004
class CButton :
	public CBrick
{
	bool handled;
public:
	CButton(float x, float y) : CBrick(x, y)
	{
		handled = false;
		SetState(BUTTON_STATE_NORMAL);
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	int IsBlocking();
};

