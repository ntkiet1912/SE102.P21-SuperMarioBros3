#include "InvisibleBlock.h"
#include "Textures.h"
#include "Game.h"

void CInvisibleBlock::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	CGame::GetInstance()->Draw(x - cx, y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CInvisibleBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - (this->cellWidth * this->length) / 2 + 2;
	t = y - this->cellHeight / 2;
	r = x + (this->cellWidth * this->length) / 2 - 2;
	b = t + this->cellHeight;
}
void CInvisibleBlock::Render()
{
	//RenderBoundingBox();
}

int CInvisibleBlock::IsDirectionColliable(float nx, float ny)
{
	
	if (nx == 0 && ny == -1 && isBlock == 0)
		return 1;

	
	if (
		((nx == -1 && ny == 0) ||  
			(nx == 1 && ny == 0) ||  
			(nx == 0 && ny == -1) ||  
			(nx == 0 && ny == 1))     
		&& isBlock == 1)
		return 1;

	return 0;
}
