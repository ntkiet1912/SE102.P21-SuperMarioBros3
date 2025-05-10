#include "FireBullet.h"
#include "Animations.h"
#include "Game.h"
#include "Mario.h"
#include <cstdlib>

#define DEG2RAD(deg) ((deg) * 3.14159265f / 180.0f)


CFireBullet::CFireBullet(float x, float y, float targetX, float targetY)
{
    this->x = x;
    this->y = y;

    float dx = targetX - x;
    float dy = targetY - y;

    float length = sqrtf(dx * dx + dy * dy);
    if (length == 0) length = 1.0f;

    // Thêm độ lệch nhỏ vào hướng bắn (ví dụ: ±5 độ)
    float angle = atan2f(dy, dx);
    float spread = (rand() % 1001 / 1000.0f - 0.5f) * 2.0f * DEG2RAD(5.0f); // ±5 độ lệch

    angle += spread;

    vx = cosf(angle) * FIRE_BULLET_SPEED;
    vy = sinf(angle) * FIRE_BULLET_SPEED;

    createTime = GetTickCount();
}
bool CheckAABB(float l1, float t1, float r1, float b1,
    float l2, float t2, float r2, float b2)
{
    return !(r1 < l2 || r2 < l1 || b1 < t2 || b2 < t1);
}
void CFireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    x += vx * dt;
    y += vy * dt;
    
    CGameObject::Update(dt, coObjects);

    
    for (auto obj : *coObjects) {
        if (dynamic_cast<CMario*>(obj)) {
            float l1, t1, r1, b1;
            float l2, t2, r2, b2;
            GetBoundingBox(l1, t1, r1, b1);
            obj->GetBoundingBox(l2, t2, r2, b2);

            if (CheckAABB(l1, t1, r1, b1, l2, t2, r2, b2)) {
               
                CMario* mario = dynamic_cast<CMario*>(obj);

                if (!mario->getIsUntouchable())
                {
                    if (mario->getLevel() == MARIO_LEVEL_WITH_TAIL)
                    {
                        mario->SetLevel(MARIO_LEVEL_BIG);
                        mario->StartUntouchable();
                    }
                    else if (mario->getLevel() == MARIO_LEVEL_BIG)
                    {
                        mario->SetLevel(MARIO_LEVEL_SMALL);
                        mario->StartUntouchable();

                    }
                    else
                    {
                        DebugOut(L">>> Mario DIE >>> \n");
                        mario->SetState(MARIO_STATE_DIE);
                    }
                }
                this->Delete(); 
            }
        }
    }

    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CFireBullet::Render()
{
    CAnimations::GetInstance()->Get(ID_ANI_FIRE_BULLET)->Render(x, y);
	//RenderBoundingBox();
}
//void CFireBullet::OnNoCollision(DWORD dt)
//{
//	x += vx * dt;
//	y += vy * dt;
//}
void CFireBullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - BULLET_BBOX_WIDTH/2;
    t = y - BULLET_BBOX_HEIGHT/2;
    r = l + BULLET_BBOX_WIDTH;
    b = t + BULLET_BBOX_HEIGHT;
}


bool CFireBullet::IsExpired()
{
    return GetTickCount() - createTime > FIRE_BULLET_LIFETIME;
}


