#pragma once
#include "GameObject.h"

#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_GRAVITY_FLYING 0.0012f
#define GOOMBA_WALKING_SPEED 0.054f
#define GOOMBA_FLYING_SPEED 0.35f
#define GOOMBA_BOUNCE_SPEED 0.2f

#define GOOMBA_VX_DIE_SPEED 0.1f
#define GOOMBA_DYING_SPEED 0.2f
#define GOOMBA_GRAVITY_DYING 0.0003f


#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 7
#define GOOMBA_BBOX_WING_HEIGHT 19.5

#define GOOMBA_DIE_TIMEOUT 500

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_DIE_BY_COLLISION 201
#define GOOMBA_STATE_NO_WING 210
#define GOOMBA_STATE_YES_WING 211

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001
#define ID_ANI_GOOMBA_DIE_BY_COLLISION 5002

#define ID_ANI_RED_GOOMBA_NOWING_WALKING 5100
#define ID_ANI_RED_GOOMBA_YESWING_WALKING 5101
#define ID_ANI_RED_GOOMBA_YESWING_START_FLYING 5102
#define ID_ANI_RED_GOOMBA_YESWING_FLYING 5103
#define ID_ANI_RED_GOOMBA_DIE 5108
#define ID_ANI_RED_GOOMBA_DIE_BY_COLLISION 5109

class CGoomba : public CGameObject
{
protected:
	float ax;				
	float ay; 

	ULONGLONG die_start;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public: 	
	CGoomba(float x, float y);
	virtual void SetState(int state);
};

class CRedGoomba : public CGoomba
{
protected:

	bool isFlyingUp;
	bool isBoucingUp;
	int bounceCount;
	bool isWalking;
	ULONGLONG detect_start;
	ULONGLONG flying_start;
	ULONGLONG bouncing_start;
	ULONGLONG walking_start;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

public:
	CRedGoomba(float x, float y);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);

};