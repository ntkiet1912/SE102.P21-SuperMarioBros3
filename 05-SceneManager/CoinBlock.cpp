#include "CoinBlock.h"
#include "debug.h"
CCoinBlock::CCoinBlock(float x, float y, int initState)
{
    this->x = x;
    this->y = y;
    this->state = initState;
    this->isConverted = false;
}

void CCoinBlock::SetState(int state)
{
    
    if (isConverted && state == COIN_BLOCK_STATE_BRICK)
        return;

    this->state = state;

    if (state == COIN_BLOCK_STATE_BRICK)
        isConverted = true;
}

void CCoinBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
   
}

void CCoinBlock::Render()
{
    int aniId = -1;
	if (state == COIN_BLOCK_STATE_COIN)
		aniId = ID_ANI_COIN_BLOCK;
	else if (state == COIN_BLOCK_STATE_BRICK)
		aniId = ID_ANI_BRICK_BLOCK;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CCoinBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x;
    t = y;
    r = x + COINBLOCK_BBOX_WIDTH;
    b = y + COINBLOCK_BBOX_HEIGHT;
}
