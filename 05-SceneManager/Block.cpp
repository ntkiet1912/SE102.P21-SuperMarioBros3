#include "Block.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"

void CBlock::RenderBoundingBox() 
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

	float xx = x - this->cellWidth / 2 + rect.right / 2;

	CGame::GetInstance()->Draw(xx - cx, y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}
void CBlock::Render()
{
	CSprites* s = CSprites::GetInstance();
	for (size_t i = 0; i < positions.size(); ++i)
	{
		float xx = x + positions[i].first;
		float yy = y + positions[i].second;
		s->Get(sprites[i])->Draw(xx, yy);
	}
	//RenderBoundingBox();
}
void CBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->cellWidth / 2;
	float totalHeight = this->cellHeight * this->length;

	l = x - cellWidth_div_2;                
	r = x + cellWidth_div_2;                
	t = y - totalHeight + this->cellHeight/2;                
	b = y + totalHeight;                
