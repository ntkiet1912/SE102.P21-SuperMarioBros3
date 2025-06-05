#include "Boomerang.h"
#include "BoomerangBro.h"
#include "debug.h"
CBoomerang::CBoomerang(float x, float y, int direction , CBoomerangBro* owner) {
    this->x = x;
    this->y = y;
    this->startX = x;
    this->startY = y;
    this->direction = direction;
    this->vx = direction * BOOMERANG_SPEED_X;
    this->vy = -BOOMERANG_SPEED_Y; 
    this->maxDistance = BOOMERANG_MAX_DISTANCE;
    this->attackState = 0;
    this->returning = false;
	this->owner = owner; 
	if (direction > 0) {
		this->SetState(BOOMERANG_STATE_RIGHT);
	}
	else {
		this->SetState(BOOMERANG_STATE_LEFT);
	}
}

void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
    switch (attackState)
    {
    case 0: // Bay lên, hướng theo chiều vx, vy âm (bay lên)
    {
        // Bay lên đến giới hạn độ cao (ví dụ y <= startY - 50)
        if (y <= startY - 50) {
            attackState = 1;
            vy = BOOMERANG_SPEED_Y;  // bắt đầu rơi xuống
        }
        else {
            vy = -BOOMERANG_SPEED_Y;  // tiếp tục bay lên
        }
        vx = direction * BOOMERANG_SPEED_X;
        break;
    }
    case 1: // Bay xuống, hướng vẫn chiều ban đầu
    {
        vy = BOOMERANG_SPEED_Y;
        vx = direction * BOOMERANG_SPEED_X;

        // Khi bay đủ maxDistance, bắt đầu quay lại
        float distance = abs(x - startX);
        if (distance >= maxDistance) {
            attackState = 2;
            returning = true;
            vx = -vx;  // đảo chiều để quay về
        }
        break;
    }
    case 2: // Quay về chủ nhân
    {
        vy = 0; // bay ngang về
        // bay ngang về hướng ngược lại
        vx = -direction * BOOMERANG_SPEED_X;

        // Nếu boomerang gần chủ nhân (khoảng cách nhỏ), kết thúc
        if (abs(x - owner->getX()) <= 10.0f && abs(y - owner->getY()) <= 10.0f) {
            owner->NotifyBoomerangReturned();
            this->Delete(); // hoặc set trạng thái inactive để xóa
        }
        break;
    }
    }

    // Cập nhật vị trí
    x += vx * dt;
    y += vy * dt;
}
void CBoomerang::GetBoundingBox(float& l, float& t, float& r, float& b) {
    l = x - 16/2;
    t = y - 16/2;
    r = l + 16; // width
    b = t + 16; // height
}

void CBoomerang::Render() {
	int ani = -1 ;
	if (state == BOOMERANG_STATE_RIGHT) {
		ani = ID_ANI_BOOMERANG_RIGHT;
	}
	else if (state == BOOMERANG_STATE_LEFT) {
		ani = ID_ANI_BOOMERANG_LEFT;
	}
    CAnimations::GetInstance()->Get(ani)->Render(x, y);
}

