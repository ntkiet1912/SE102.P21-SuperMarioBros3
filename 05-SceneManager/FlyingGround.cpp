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
    isStepped = false;
	this->vx = 0.02f;
	this->vy = 0;
}
void CFlyingGround::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (isStepped)
    {
		// If Mario has stepped on the platform, it should move vertically
        this->vy = FLYINGGROUND_SPEED_VY;
        this->vx = 0;
    }
    else
    {
        this->vx = FLYINGGROUND_SPEED_VX; // Move horizontally
    }
    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CFlyingGround::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CFlyingGround::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    float cellWidth_div_2 = this->cellWidth / 2;
    l = x - cellWidth_div_2;
    t = y - this->cellHeight / 2 ;
    r = l + this->cellWidth * this->length;
    b = t + this->cellHeight;
}