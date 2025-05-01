#pragma once
#include "GameObject.h"
#define ID_ANI_MUSHROOM	-4000
#define ID_ANI_LEAF_LEFT	-4100
#define ID_ANI_LEAF_RIGHT	-4101
#define MUSHROOM_BBOX_HEIGHT 16
#define MUSHROOM_BBOX_WIDTH 16

#define LEFT_BBOX_HEIGHT 14
#define LEFT_BBOX_WIDTH 16

#define UPGRADE_LEVEL_GRAVITY 0.0006f
#define UPGRADE_LEVEL_SPEED 0.05f
class CUpgradeLevel :
	public CGameObject
{
protected:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) = 0;
	virtual void Render() = 0;
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CUpgradeLevel(float x, float y) : CGameObject(x, y) {}
};


class CMushroom : public CUpgradeLevel
{
protected:
	float ax ;
	float ay ;

	// based on original game, the mushroom vx when spawn will be opposite to the mario' Nx
	// so we need this variable to handle the event
	bool spawnAndMoveToLeft;
	float spawnY;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
public:
	CMushroom(float x, float y, bool spawnAndMoveToLeft);
};

class CLeaf : public CUpgradeLevel
{
protected:
	float ax;
	float ay;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	CLeaf(float x, float y);
};

