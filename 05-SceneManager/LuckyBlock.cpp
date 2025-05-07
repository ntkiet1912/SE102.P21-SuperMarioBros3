#include "LuckyBlock.h"
#include "debug.h"
#include "Mario.h"
#include "PlayScene.h"
#include "UpgradeMarioLevel.h"
#include "Coin.h"
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
void CLuckyBlock::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}
void CLuckyBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - LUCKYBLOCK_BBOX_WIDTH / 2;
	t = y - LUCKYBLOCK_BBOX_HEIGHT / 2 ;
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
		spawnItem();
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

void CLuckyBlock::spawnItem()
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CGameObject* item = nullptr;
	switch (containItemIndex)
	{
	case ITEM_UPGRADELEVEL:
		if (mario->getLevel() == 1)
		{
			// mushroom always want to move to the opposite side of Mario 
			// so new variable spawnAndMoveToLeft is added
			if (mario->getX() > this->x)
			{
				item = new CMushroomUpgradingMarioLevel(x, y, true);
			}
			else
			{
				item = new CMushroomUpgradingMarioLevel(x, y, false);

			}
		}
		else if (mario->getLevel() > 1)
		{
			item = new CLeaf(x, y);
		}
		break;
	case ITEM_COIN:
		item = new CCoinFromLuckyBlock(x, y - 8);
		break;
	case ITEM_1UP:
		if (mario->getX() > this->x)
		{
			item = new CMushroom1UP(x, y, true);
		}
		else
		{
			item = new CMushroom1UP(x, y, false);
		}
		break;
	}

	if (item)
	{
		// call current scene to add item to Objects
		CScene* scene = CGame::GetInstance()->GetCurrentScene();
		((CPlayScene*)scene)->AddObject(item);
	}
}