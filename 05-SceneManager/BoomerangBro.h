#pragma once
#include "GameObject.h"

#define BOOMERANG_BRO_GRAVITY           0.001f
#define BOOMERANG_BRO_WALKING_SPEED     0.04f
#define BOOMERANG_BRO_THROW_INTERVAL    2000
#define BOOMERANG_SPEED                 0.15f

#define BOOMERANG_BRO_BBOX_WIDTH        17
#define BOOMERANG_BRO_BBOX_HEIGHT       20

#define BOOMERANG_BRO_STATE_WALKING     3000
#define BOOMERANG_BRO_STATE_THROWING    3001
#define BOOMERANG_BRO_STATE_DIE         3002

#define ID_ANI_BOOMERANG_BRO_WALKING_LEFT     31001
#define ID_ANI_BOOMERANG_BRO_WALKING_RIGHT    31002
#define ID_ANI_BOOMERANG_BRO_THROWING_LEFT    31003
#define ID_ANI_BOOMERANG_BRO_THROWING_RIGHT   31004
#define ID_ANI_BOOMERANG_BRO_DIE              31005

class CBoomerangBro : public CGameObject
{
protected:
    float ax;
    float ay;
    float startX;
    float patrolRange = 100.0f;
    ULONGLONG lastThrowTime = 0;
    bool isThrowing = false;
	int direction = 1; // 1 for right, -1 for left
    bool isBoomerangFlying = false;

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Render();

    virtual int IsCollidable() { return 1; };
    virtual int IsBlocking() { return 0; };
    virtual void OnNoCollision(DWORD dt);
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);
    void ThrowBoomerang(); 

public:
    CBoomerangBro(float x, float y);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void SetState(int state);
	void NotifyBoomerangReturned(); 
    
};
