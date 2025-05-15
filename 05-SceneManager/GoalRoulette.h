#pragma once
#define MUSHROOM_GOALROULETTE 1
#define FLOWER_GOALROULETTE 2
#define STAR_GOALROULETTE 3
#define COURSECLEAR_GOALROULETTE 4
#define YOUGOTACARD_GOALROULETTE 5

#define GR_ICON_WIDTH_BBOX 16
#define GR_ICON_HEIGHT_BBOX 16

#define MUSHROOM_STATIC	-20000
#define FLOWER_STATIC	-20010
#define STAR_STATIC	-20020

#define MUSHROOM_NON_STATIC	-20001
#define FLOWER_NON_STATIC	-20011
#define STAR_NON_STATIC	-20021

#define MUSHROOM_CARD	-20002
#define FLOWER_CARD	-20012
#define STAR_CARD -20022

#define COURSE_CLEAR -20118
#define YOU_GOT_NEW_CARD -20119

#include "GameObject.h"

class CGoalRoulette : public CGameObject
{
protected:
	int type;
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {}

public:
	CGoalRoulette(float x, float y, int type) :CGameObject(x, y), type(type) {}
};

class CGoalRouletteIcon :
	public CGameObject
{
protected:
	bool isHitByMario;
	int iconType;
	ULONGLONG timeToSwitching;
	ULONGLONG deleteTime;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; };
	void OnNoCollision(DWORD dt);
public:
	CGoalRouletteIcon(float x, float y);
	void setIsHitByMario(bool isHit) { isHitByMario = isHit; }
};