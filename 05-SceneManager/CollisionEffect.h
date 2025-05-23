#pragma once
#include "GameObject.h"
class CCollisionEffect : public CGameObject
{
    ULONGLONG startTime;
    DWORD lifeTime = 50;

public:
    CCollisionEffect(float x, float y)
    {
        this->x = x;
        this->y = y;
        startTime = GetTickCount64();
    }

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
    {
        if (GetTickCount64() - startTime > lifeTime)
            isDeleted = true;
    }
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom){}
    void Render()
    {
        int aniId = 0;
        CAnimations::GetInstance()->Get(aniId)->Render(x, y);
    }
};

