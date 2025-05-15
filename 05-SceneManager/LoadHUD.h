#pragma once
#include "Sprite.h"

class CLoadHUD
{
	LPSPRITE sprite;
	float x, y;

public: 
	CLoadHUD(LPSPRITE sprite, float x, float y) : sprite(sprite), x(x), y(y) {}
	void Draw();
	void SetSprite(LPSPRITE sprite) { this->sprite = sprite; }
};