#pragma once
#include "GameObject.h"
#include "Koopas.h"
#include "Goomba.h"
#include "Mario.h"
#define DEAD_ZONE_L 0
#define DEAD_ZONE_T 455
#define DEAD_ZONE_R 3000
#define DEAD_ZONE_B 470

class CDeadZone :
	public CGameObject
{
public:
	void Render() {}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
	{
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b)
	{
		l = DEAD_ZONE_L;
		t = DEAD_ZONE_T;
		r = DEAD_ZONE_R;
		b = DEAD_ZONE_B;
	}
};
