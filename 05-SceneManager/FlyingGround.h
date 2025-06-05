#pragma once
#include "Platform.h"
#define FLYINGGROUND_SPEED_VY 0.035f
#define FLYINGGROUND_SPEED_VX -0.025f
class CFlyingGround :
	public CPlatform
{
protected:
	bool isStepped = false; // Flag to check if Mario has stepped on the platform
public:
	CFlyingGround(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	//void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnNoCollision(DWORD dt);
	int IsBlocking() { return 1; } // Flying ground is not blocking
	int IsColliable() { return 1; }
	int IsDirectionColliable(float nx, float ny) { return 1; }
	void setIsStepped(bool isStepped) { this->isStepped = isStepped; }
	bool getIsStepped() { return isStepped; }

	void GetBoundingBox(float& l, float& t, float& r, float& b);
};
