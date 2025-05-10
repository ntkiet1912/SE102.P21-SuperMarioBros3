#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define FIRE_PIRANHA_WIDTH 16
#define FIRE_PIRANHA_HEIGHT 33

#define PIRANHA_RISE_SPEED 0.02f
#define PIRANHA_FIRE_DELAY 500	
#define PIRANHA_HIDE_DELAY 2000
#define PIRANHA_RISE_DELAY 2000
#define PIRANHA_MAX_RISE 33.0f

#define PIRANHA_STATE_FIRE_UP_LEFT 100
#define PIRANHA_STATE_FIRE_UP_RIGHT 200
#define PIRANHA_STATE_FIRE_DOWN_LEFT	300
#define PIRANHA_STATE_FIRE_DOWN_RIGHT 400

class CFirePiranha : public CGameObject
{
protected:
	float startY;
	float targetY;
	ULONGLONG stateTimer;
	int direction;
	int facingVertical;
	bool hasFired = false;

	void UpdateDirection();
	void Fire();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	bool IsPlayerTooClose();
	int IsBlocking() { return 0; }
	int IsCollidable() { return 1; }
public:
	CFirePiranha(float x, float y);
	virtual void SetState(int state);
};
