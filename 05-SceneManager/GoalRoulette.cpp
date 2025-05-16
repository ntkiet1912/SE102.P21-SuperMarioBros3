#include "GoalRoulette.h"
#include "debug.h"
#include "PlayScene.h"
#include "Decoration.h"
void CGoalRoulette::Render()
{
	int aniId = -1;
	if (type == COURSECLEAR_GOALROULETTE)
		aniId = COURSE_CLEAR;
	else if (type == YOUGOTACARD_GOALROULETTE)
		aniId = YOU_GOT_NEW_CARD;
	else if (type == MUSHROOM_GOALROULETTE)
		aniId = MUSHROOM_CARD;
	else if (type == FLOWER_GOALROULETTE)
		aniId = FLOWER_CARD;
	else if(type == STAR_GOALROULETTE)
		aniId = STAR_CARD;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}


CGoalRouletteIcon::CGoalRouletteIcon(float x, float y) :CGameObject(x, y)
{
	vy = 0;
	isHitByMario = false;
	timeToSwitching = GetTickCount64();
	iconType = MUSHROOM_GOALROULETTE;
	deleteTime = -1;
}
void CGoalRouletteIcon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!isHitByMario)
	{
		left = x - GR_ICON_WIDTH_BBOX / 2;
		top = y - GR_ICON_HEIGHT_BBOX / 2;
		right = x + GR_ICON_WIDTH_BBOX / 2;
		bottom = y + GR_ICON_HEIGHT_BBOX / 2;
	}
	else
	{
		left = top = right = bottom = 0;
	}
}
void CGoalRouletteIcon::OnNoCollision(DWORD dt)
{
	y += vy * dt;
}

void CGoalRouletteIcon::spawnGoalRouletteObjects()
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	vector<LPGAMEOBJECT>& objects = scene->GetObjects();

	objects.push_back(new CGoalRoulette(2510, 70, iconType));
	objects.push_back(new CGoalRoulette(2438, 48, COURSECLEAR_GOALROULETTE));
	objects.push_back(new CGoalRoulette(2428, 62, YOUGOTACARD_GOALROULETTE));
}

void CGoalRouletteIcon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isHitByMario)
	{
		vy = -0.08f;
	}
	else
	{
		// change super fast between 3 types: mushroom, flower, star
		if (GetTickCount64() - timeToSwitching > 100)
		{
			timeToSwitching = GetTickCount64();
			iconType++;
			//outta bound ignorance 
			if (iconType > 3) iconType = 1;
		}
	}

	// when out of map, appearing 2 sentences and 1 card
	if (y < static_cast<float>(-GR_ICON_HEIGHT_BBOX) / 2)
	{
		deleteTime = GetTickCount64();
		spawnGoalRouletteObjects();
	}
	// need to seperate it into a small function because it causes this error below
	// Exception thrown at 0x793B896F (d3dx9d_43.dll) in 05-SceneManager.exe:
	// 0xC0000005: Access violation reading location 0x00000080.
	if(GetTickCount64() - deleteTime < 1000)
	{
		isDeleted = true;

	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CGoalRouletteIcon::Render()
{
	int aniId = -1;
	if (iconType == MUSHROOM_GOALROULETTE)
	{
		if (isHitByMario)
			aniId = MUSHROOM_NON_STATIC;
		else 
			aniId = MUSHROOM_STATIC;
	}
	else if (iconType == FLOWER_GOALROULETTE)
	{
		if (isHitByMario)
			aniId = FLOWER_NON_STATIC;
		else
			aniId = FLOWER_STATIC;
	}
	else
	{
		if (isHitByMario)
			aniId = STAR_NON_STATIC;
		else
			aniId = STAR_STATIC;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}