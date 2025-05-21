#pragma once
#include "GameObject.h"
#include "Collision.h"

#define TAIL_WIDTH 11
#define TAIL_HEIGHT 4
class CTail :
    public CGameObject
{
public:
    bool isActive = false;
    CTail(float x, float y) :CGameObject(x, y) { isActive = false; }
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) 
    {
        CCollision::GetInstance()->Process(this, dt, coObjects);
    }
    void Render() 
    {
        RenderBoundingBox();
    }
    virtual int IsCollidable() { return 1; };
    virtual int IsBlocking() { return 0; }
    void GetBoundingBox(float& l, float& t, float& r, float& b);
    void OnCollisionWith(LPCOLLISIONEVENT e);
    void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
    void OnCollisionWithLuckyBlock(LPCOLLISIONEVENT e);
};
