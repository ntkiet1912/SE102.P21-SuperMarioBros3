#pragma once
#include "GameObject.h"
#include "BoomerangBro.h"

#define BOOMERANG_SPEED_X 0.06f
#define BOOMERANG_SPEED_Y 0.05f
#define BOOMERANG_GRAVITY 0.0002f
#define BOOMERANG_MAX_DISTANCE 80.0f

#define BOOMERANG_STATE_RIGHT 4001
#define BOOMERANG_STATE_LEFT 4002

#define ID_ANI_BOOMERANG_RIGHT 40001
#define ID_ANI_BOOMERANG_LEFT 40002

class CBoomerang : public CGameObject
{
protected:
	float startX, startY;
	float maxDistance;
	bool returning;
	int attackState; // 0: bay lên, 1: bay xuống, 2: quay về
	int direction; // 1: phải, -1: trái
	CBoomerangBro* owner;

public:
	CBoomerang(float x, float y, int direction , CBoomerangBro* owner);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetReturning(bool value) { returning = value; }
};