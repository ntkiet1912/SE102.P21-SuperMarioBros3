#include "UpgradeMarioLevel.h"
#include "debug.h"
void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - LEFT_BBOX_WIDTH / 2;
	r = x + LEFT_BBOX_WIDTH / 2;
	t = y - LEFT_BBOX_HEIGHT / 2;
	b = y + LEFT_BBOX_HEIGHT / 2;
}
CLeaf::CLeaf(float x, float y) : CUpgradeLevel(x, y)
{
	boundOfMovingMaxToTheLeft = x - 24;
	boundOfMovingMaxToTheRight = x + 24;
	ax = 0;
	ay = 0;

	// when luckybox is hit, the leaf will bounce to the sky
	vy = LEAF_SPAWNING_SPEED_VY;
	spawnY = y - 48;
}
void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += ax * dt;
	vy += ay * dt;
	// completely spawned
	if (y <= spawnY)
	{
		spawnY =  -999;
		vy = 0;
		ay = LEAF_GRAVITY;
		ax = LEAF_ACCELARATION;
		vx = LEAF_MOVING_SPEED;
	}
	// bound right -> make it go back to the left 
	if (x >= boundOfMovingMaxToTheRight)
	{
		ax = -LEAF_ACCELARATION;
		ay = LEAF_GRAVITY;

		vx = -LEAF_MOVING_SPEED;
		nx = -1;
		vy = 0;
	}
	// bound left -> make it go back to the right
	else if (x <= boundOfMovingMaxToTheLeft)
	{
		vx = LEAF_MOVING_SPEED;
		vy = 0;
		ay = LEAF_GRAVITY;
		nx = 1;
		ax = LEAF_ACCELARATION;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

}
void CLeaf::Render()
{
	int aniId;
	if (nx >= 0) aniId = ID_ANI_LEAF_RIGHT;
	else aniId= ID_ANI_LEAF_LEFT;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CLeaf::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CLeaf::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking())
		return;
}

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
	vy = MUSHROOM_SPAWNING_SPEED_VY;
	spawnY = y - 16;
}
void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += ax * dt;
	vy += ay * dt;

	if (y <= spawnY)
	{
		if (spawnAndMoveToLeft)
			vx = -MUSHROOM_MOVING_SPEED;
		else vx = MUSHROOM_MOVING_SPEED;

		vy = MUSHROOM_SPEED_VY;
		ay = MUSHROOM_GRAVITY;
		spawnY = -999;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}
void CMushroom1UP::Render()
{
	int aniId = ID_ANI_MUSHROOM1UP;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CMushroomUpgradingMarioLevel::Render()
{
	int aniId = ID_ANI_MUSHROOM;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}