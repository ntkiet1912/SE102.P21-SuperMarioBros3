#pragma once
#include "Sprite.h"
class CBackgroundTile
{
	LPSPRITE sprite;
	float x, y;
public:
	CBackgroundTile(LPSPRITE sprite, float x, float y) : sprite(sprite), x(x), y(y) {}
	void Draw() { sprite->Draw(x, y); }
};

