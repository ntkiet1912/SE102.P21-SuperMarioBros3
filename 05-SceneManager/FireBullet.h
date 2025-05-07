#pragma once
#include "GameObject.h"

#define ID_ANI_FIRE_BULLET 25000

#define BULLET_BBOX_WIDTH 8
#define BULLET_BBOX_HEIGHT 8

#define FIRE_BULLET_SPEED 0.08f
#define FIRE_BULLET_LIFETIME 3000 // milliseconds

class CFireBullet : public CGameObject
{
protected:
    float vx, vy;
    DWORD createTime;

public:
    CFireBullet(float x, float y, float targetX, float targetY);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();

    void GetBoundingBox(float& l, float& t, float& r, float& b);
    bool IsExpired();

};
