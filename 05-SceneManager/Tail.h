#pragma once
#include "GameObject.h"
#include "Collision.h"

#define TAIL_WIDTH 16
#define TAIL_HEIGHT 4
class CTail :
    public CGameObject
{
public:
    bool isActive = false;
    //float ax;
    CTail(float x, float y, int nx) :CGameObject(x, y) 
    { 
        isActive = false;
        this->nx = nx;
        //ax = nx > 0 ? 0.0002f : -0.0002f;
    }
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render() 
    {
        RenderBoundingBox();
    }
    virtual int IsCollidable() { return 1; };
    virtual int IsBlocking() { return 0; }
    void GetBoundingBox(float& l, float& t, float& r, float& b);
    void OnNoCollision(DWORD dt);
    void OnCollisionWith(LPCOLLISIONEVENT e);
    void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
    void OnCollisionWithLuckyBlock(LPCOLLISIONEVENT e);
    void OnCollisionWithKoopas(LPCOLLISIONEVENT e);
};
