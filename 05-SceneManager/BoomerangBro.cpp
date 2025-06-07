#include "BoomerangBro.h"
#include "Boomerang.h" // Giả định bạn có class Boomerang riêng
#include "PlayScene.h"
#include "debug.h"

CBoomerangBro::CBoomerangBro(float x, float y) {
    this->x = x;
    this->y = y;
    this->ax = 0;
    this->ay = BOOMERANG_BRO_GRAVITY;
    this->vx = -BOOMERANG_BRO_WALKING_SPEED;
	this->startX = x;
    direction = -1;
    SetState(BOOMERANG_BRO_STATE_WALKING);
}

void CBoomerangBro::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x - BOOMERANG_BRO_BBOX_WIDTH / 2;
    top = y - BOOMERANG_BRO_BBOX_HEIGHT / 2;
    right = left + BOOMERANG_BRO_BBOX_WIDTH;
    bottom = top + BOOMERANG_BRO_BBOX_HEIGHT;
}

void CBoomerangBro::Render() {
    int ani = -1;

    if (state == BOOMERANG_BRO_STATE_DIE) ani = ID_ANI_BOOMERANG_BRO_DIE;
    else if (vx < 0)
        ani = isThrowing ? ID_ANI_BOOMERANG_BRO_THROWING_LEFT : ID_ANI_BOOMERANG_BRO_WALKING_LEFT;
    else
        ani = isThrowing ? ID_ANI_BOOMERANG_BRO_THROWING_RIGHT : ID_ANI_BOOMERANG_BRO_WALKING_RIGHT;

    CAnimations::GetInstance()->Get(ani)->Render(x, y);
    //RenderBoundingBox();
}

void CBoomerangBro::OnNoCollision(DWORD dt) {
    x += vx * dt;
    y += vy * dt;
}

void CBoomerangBro::OnCollisionWith(LPCOLLISIONEVENT e) {
    if (!e->obj->IsBlocking()) return;

    if (e->ny != 0) vy = 0;
    else if (e->nx != 0) vx = -vx;
}

void CBoomerangBro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
    vy += ay * dt;

    if (x > startX + patrolRange) {
        x = startX + patrolRange;
        vx = -abs(vx); 
    }
    else if (x < startX - patrolRange) {
        x = startX - patrolRange;
		vx = abs(vx);
    }

	LPGAME game = CGame::GetInstance();
	LPGAMEOBJECT player = ((CPlayScene*)game->GetCurrentScene())->GetPlayer();

    if (player != nullptr) {
        direction = (player->getX() > x) ? 1 : -1;
    }

    if (!isBoomerangFlying &&  GetTickCount() - lastThrowTime >= BOOMERANG_BRO_THROW_INTERVAL) {
        ThrowBoomerang();
		isThrowing = true; // Đặt trạng thái là đang ném
		DebugOut(L"[BoomerangBro] Throwing boomerang at position: (%.2f, %.2f)\n", x, y);
        lastThrowTime = GetTickCount();
    }
    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CBoomerangBro::ThrowBoomerang() {

    LPGAMEOBJECT boomerang = new CBoomerang(x, y, direction, this); 
    isBoomerangFlying = true;
    
    ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddObject(boomerang);
}
void CBoomerangBro::NotifyBoomerangReturned() {
    isBoomerangFlying = false;
    isThrowing = false;
}

void CBoomerangBro::SetState(int state) {
    CGameObject::SetState(state);

    switch (state) {
    case BOOMERANG_BRO_STATE_DIE:
        vx = 0;
        vy = 0;
        break;
    case BOOMERANG_BRO_STATE_WALKING:
        vx = -BOOMERANG_BRO_WALKING_SPEED;
        break;
    case BOOMERANG_BRO_STATE_THROWING:
        isThrowing = true;
        vx = -BOOMERANG_BRO_WALKING_SPEED;
        break;
    }
}
