#include "UpgradeMarioLevel.h"

void CUpgradeLevel::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CUpgradeLevel::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	//if (dynamic_cast<CGoomba*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

// find that mushroom's and the leaf's boundingbox are not the same so seperate them into 2 function
void CMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - MUSHROOM_BBOX_WIDTH / 2;
	r = x + MUSHROOM_BBOX_WIDTH / 2;
	t = y - MUSHROOM_BBOX_HEIGHT / 2;
	b = y + MUSHROOM_BBOX_HEIGHT / 2;
}
CMushroom::CMushroom(float x, float y, bool spawnAndMoveToLeft) :CUpgradeLevel(x, y)
{
	this->spawnAndMoveToLeft = spawnAndMoveToLeft;
	ax = 0;
	ay = 0;
	vy = -0.035f;
	spawnY = y - 16;
}
void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += ax * dt;
	vy += ay * dt;

	if (y <= spawnY)
	{
		if (spawnAndMoveToLeft)
			vx = -0.06f;
		else vx = 0.06f;

		ax = 0;
		ay = UPGRADE_LEVEL_GRAVITY;
		vy = 0.02f;
		spawnY = -999;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMushroom::Render()
{
	int aniId = ID_ANI_MUSHROOM;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}


// find that mushroom's and the leaf's boundingbox are not the same so seperate them into 2 function
void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - LEFT_BBOX_WIDTH / 2;
	r = x + LEFT_BBOX_WIDTH / 2;
	t = y - LEFT_BBOX_HEIGHT / 2;
	b = y + LEFT_BBOX_HEIGHT / 2;
}
CLeaf::CLeaf(float x, float y) : CUpgradeLevel(x, y)
{
	ax = 0;
	ay = UPGRADE_LEVEL_GRAVITY;
}
void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += ax * dt;
	vy += ay * dt;
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

}

void CLeaf::Render()
{
	int aniId;
	if (nx >= 0) aniId = ID_ANI_LEAF_RIGHT;
	else aniId= ID_ANI_LEAF_LEFT;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}
