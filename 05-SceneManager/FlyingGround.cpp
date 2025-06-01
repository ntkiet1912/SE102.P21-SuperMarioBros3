#include "FlyingGround.h"
#include "GameObject.h"
#include "Platform.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"
#include "Mario.h"

CFlyingGround::CFlyingGround(float x, float y,
	float cell_width, float cell_height, int length,
	int sprite_id_begin, int sprite_id_middle, int sprite_id_end) :
	CPlatform(x, y, cell_width, cell_height, length, sprite_id_begin, sprite_id_middle, sprite_id_end)
{
	this->vx = 0.02f;
	this->vy = 0;
}
void CFlyingGround::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isStepped)
	{
		this->vx = 0;
		ay = FLYINGGROUND_GRAVITY;
		vy += ay * dt; // Apply gravity to the platform
	}
	else
	{
		this->vx = FLYINGGROUND_SPEED_VX;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
//void CFlyingGround::OnCollisionWith(LPCOLLISIONEVENT e)
//{
//	CMario* mario = dynamic_cast<CMario*>(e->obj);
//	if (mario)
//	{
//		if (e->ny != 0) // Mario is on top of the platform
//		{
//			isStepped = true; // Set the flag to indicate Mario has stepped on the platform
//			mario->setVy(FLYINGGROUND_GRAVITY);
//		}
//	}
//}
void CFlyingGround::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}