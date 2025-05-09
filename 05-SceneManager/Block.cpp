#include "Block.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"


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



