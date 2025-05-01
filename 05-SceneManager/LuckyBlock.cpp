#include "LuckyBlock.h"
#include "debug.h"

void CLuckyBlock::Render()
{
	int aniId;
	if (isEmpty)
		aniId = ID_ANI_LUCKY_BLOCK_EMPTY;
	else
		aniId = ID_ANI_LUCKY_BLOCK_NORMAL;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CLuckyBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - LUCKYBLOCK_BBOX_WIDTH / 2;
	t = y - LUCKYBLOCK_BBOX_HEIGHT / 2;
	r = l + LUCKYBLOCK_BBOX_WIDTH;
	b = t + LUCKYBLOCK_BBOX_HEIGHT;
}
void CLuckyBlock::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case LUCKYBLOCK_NORMAL:
		isEmpty = false;
		break;
	case LUCKYBLOCK_EMPTY:
		isEmpty = true;
		break;
	default:
		break;
	}
}
void CLuckyBlock::OnCollisionWith(LPCOLLISIONEVENT e)
{
	//if (e->ny != 0)
	//{
	//	isEmpty = true;
	//}
}


void CLuckyBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	// limit the high of lb
	if (y < collisionY)
	{
		vy = LUCKYBOX_VY;
	}
	else if (isHit && !isEmpty && y > collisionY)
	{
		vy = -LUCKYBOX_VY;
		isHit = false;
		isEmpty = true;
	}
	else if (y >= initY)
	{
		vy = 0;
		// avoiding out of bound and drop out of world
		SetPosition(this->getX(), this->initY);
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CLuckyBlock::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};
