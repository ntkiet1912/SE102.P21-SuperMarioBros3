#include "FireBullet.h"
#include "Animations.h"
#include "Game.h"

CFireBullet::CFireBullet(float x, float y, float targetX, float targetY)
{
    this->x = x;
    this->y = y;

    float dx = targetX - x;
    float dy = targetY - y;

    float length = sqrtf(dx * dx + dy * dy);
    if (length == 0) length = 1.0f; 

    vx = (dx / length) * FIRE_BULLET_SPEED;
    vy = (dy / length) * FIRE_BULLET_SPEED;

    createTime = GetTickCount();
}

void CFireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    x += vx * dt;
    y += vy * dt;

}

void CFireBullet::Render()
{
    CAnimations::GetInstance()->Get(ID_ANI_FIRE_BULLET)->Render(x, y);
	RenderBoundingBox();
}

void CFireBullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x ;
    t = y;
    r = l + BULLET_BBOX_WIDTH;
    b = t + BULLET_BBOX_HEIGHT;
}

bool CFireBullet::IsExpired()
{
    return GetTickCount() - createTime > FIRE_BULLET_LIFETIME;
}


