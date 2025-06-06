#include "Coin.h"
#include "debug.h"


void CCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_COIN)->Render(x, y);

	//RenderBoundingBox();
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}

void CCoinFromLuckyBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	// reached max, dropping down 
	if (y < maxY)
	{
		reachedMaxY = true;
		vy = COIN_AFTER_REACHED_MAXY_VY;
		ay = COIN_AY;
	}
	// simulating collision with the lucky block
	// will process this in future 
	else if (reachedMaxY && y > spawnY)
	{
		this->Delete();
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CCoinFromLuckyBlock::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_LUCKYBLOCK_COIN)->Render(x, y);

	//RenderBoundingBox();
}