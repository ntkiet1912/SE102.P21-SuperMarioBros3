#include "BigPipe.h"
#include "Sprite.h"
#include "Sprites.h"
#include "Mario.h"
#include "Textures.h"
#include "Game.h"
#include "PlayScene.h"
void CBigPipe::Render()
{
	if (this->height <= 0) return;
	float yy = y;
	CSprites* s = CSprites::GetInstance();

	s->Get(headId)->Draw(x, y);

	for (int i = 1; i < this->height; i++)
	{
		yy += PIPE_CELL_HEIGHT;
		s->Get(bodyId)->Draw(x, yy);
	}

	//RenderBoundingBox();
}

void CBigPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PIPE_CELL_WIDTH / 2;
	t = y - PIPE_CELL_HEIGHT / 2;
	r = l + PIPE_CELL_WIDTH;
	b = t + PIPE_CELL_HEIGHT * this->height;
}

void CBigPipe::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);
	float yy = y + PIPE_CELL_HEIGHT * (this->height - 1) / 2;
	CGame::GetInstance()->Draw(x - cx, yy - cy, bbox, nullptr, BBOX_ALPHA, r - l, b - t);
}




