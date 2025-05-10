#pragma once
#include "GameObject.h"
#define ID_ANI_MUSHROOM	-4000
#define ID_ANI_LEAF_LEFT	-4100
#define ID_ANI_LEAF_RIGHT	-4101
#define ID_ANI_MUSHROOM1UP	-4200

#define MUSHROOM_BBOX_HEIGHT 15
#define MUSHROOM_BBOX_WIDTH 16

#define LEFT_BBOX_HEIGHT 14
#define LEFT_BBOX_WIDTH 16

#define MUSHROOM_GRAVITY 0.0006f
#define MUSHROOM_MOVING_SPEED 0.06f
#define MUSHROOM_SPAWNING_SPEED_VY -0.035f
#define MUSHROOM_SPEED_VY 0.02f

#define LEAF_GRAVITY 0.0001f
#define LEAF_MOVING_SPEED 0.02f

#define LEAF_ACCELARATION 0.00035f
#define LEAF_SPAWNING_SPEED_VY -0.135f


class CUpgradeLevel :
	public CGameObject
{
protected:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) = 0;
	virtual void Render() = 0;
	virtual int IsCollidable() { return 1; }
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt) = 0;
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) = 0;

public:
	CUpgradeLevel(float x, float y) : CGameObject(x, y) {}
};


class CMushroom : public CUpgradeLevel
{
protected:
	float ax;
	float ay;

	// based on original game, the mushroom vx when spawn will be opposite to the mario' Nx
	// so we need this variable to handle the event
	bool spawnAndMoveToLeft;
	float spawnY;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render() = 0;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CMushroom(float x, float y, bool spawnAndMoveToLeft);
};

class CLeaf : public CUpgradeLevel
{
protected:
	float ax;
	float ay;
	float boundOfMovingMaxToTheRight;
	float boundOfMovingMaxToTheLeft;
	float spawnY;

	virtual int IsCollidable() { return 0; }
	virtual int IsBlocking() { return 0; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);


public:
	CLeaf(float x, float y);
};

// 2 mushrooms are the same so devired from CMushroom
// Animation diff -> seperated it into 2 classes
class CMushroom1UP : public CMushroom
{
public:
	CMushroom1UP(float x, float y, bool spawnAndMoveToLeft) : CMushroom(x, y, spawnAndMoveToLeft) {}
	void Render() override;
};

class CMushroomUpgradingMarioLevel : public CMushroom
{
public:
	CMushroomUpgradingMarioLevel(float x, float y, bool spawnAndMoveToLeft) : CMushroom(x, y, spawnAndMoveToLeft) {}
	void Render() override;
};
