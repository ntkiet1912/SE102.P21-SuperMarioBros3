#include "LuckyBlock.h"
#include "debug.h"
#include "Mario.h"
#include "PlayScene.h"
#include "UpgradeMarioLevel.h"
#include "Coin.h"
#include "DataManager.h"
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
	t = y - LUCKYBLOCK_BBOX_HEIGHT / 2;
	r = l + LUCKYBLOCK_BBOX_WIDTH;
	b = t + LUCKYBLOCK_BBOX_HEIGHT;
}
void CLuckyBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	if (isHit)
	{
		// being hit, fly up 
		if (!isEmpty && y > maxY)
		{
			vy = -LUCKYBOX_VY;
			isEmpty = true;

			// got a flag makes mushroom spawn after the block completely turn back to its initPos
			CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			if ((this->containItemIndex == ITEM_UPGRADELEVEL && mario->getLevel() == 1) || this->containItemIndex == ITEM_1UP)
			{
				flagForMushroom = true;
			}
			if (!flagForMushroom)
			{
				spawnItem();
			}
		}
		// reach maxY, go back to original pos
		else if (y < maxY)
		{
			vy = LUCKYBOX_VY;
		}
		// go to original pos now
		else if (y >= initY)
		{
			isHit = false;
			isReturnToOriginalPos = true;
		}
	}
	if (isReturnToOriginalPos)
	{
		isReturnToOriginalPos = false;
		vy = 0;
		// avoiding out of bound and drop out of world
		SetPosition(this->getX(), this->initY);
		if (flagForMushroom)
		{
			flagForMushroom = false;
			spawnItem();
		}

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
		if (mario->getLevel() > 1)
		{
			item = new CLeaf(x, y - 8);

		}
		else
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
		break;
	case ITEM_COIN:
		item = new CCoinFromLuckyBlock(x, y - 8);
		CDataManager::GetInstance()->AddCoin(1);
		CDataManager::GetInstance()->AddScore(200);
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
		// if it is mushroom
		// add it at the previous place of the box to make it lapped by the box 
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		vector<LPGAMEOBJECT>& objects = scene->GetObjects();
		if (dynamic_cast<CMushroom*>(item))
		{
			auto it = std::find(objects.begin(), objects.end(), this);
			if (it != objects.end()) {
				objects.insert(it, item);
			}
		}
		else objects.push_back(item);
	}
}

/*******************************************************************/
void GoldenLuckyBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	if (isHit)
	{
		// being hit, fly up 
		if (!isEmpty && y > maxY)
		{
			vy = -LUCKYBOX_VY;
			if (containItemIndex == ITEM_COIN)
			{
				coinCount--;
				if (coinCount <= 0)
				{
					isEmpty = true;
				}
			}
			else
			{
				isEmpty = true;
			}

			CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

			if ((this->containItemIndex == ITEM_UPGRADELEVEL && mario->getLevel() == 1) || this->containItemIndex == ITEM_1UP)
			{
				flagForMushroom = true;
			}
			if (!flagForMushroom)
			{
				spawnItem();
			}
		}
		// reach maxY, go back to original pos
		else if (y < maxY)
		{
			vy = LUCKYBOX_VY;
		}
		// go to original pos now
		else if (y >= initY)
		{
			isHit = false;
			isReturnToOriginalPos = true;
		}
	}
	if (isReturnToOriginalPos)
	{
		isReturnToOriginalPos = false;
		vy = 0;
		// avoiding out of bound and drop out of world
		SetPosition(this->getX(), this->initY);
		if (flagForMushroom)
		{
			flagForMushroom = false;
			spawnItem();
		}
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}