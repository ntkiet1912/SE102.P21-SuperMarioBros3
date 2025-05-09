#pragma once
#include "GameObject.h"

class CInvisibleBlock : public CGameObject
{
protected:
	int length;
	float cellWidth;
	float cellHeight;
	int isBlock ; // 0: not block, 1: block
	
public:
	CInvisibleBlock(float x, float y, int length, float cellWidth, float cellHeight , int isBlock)
		: CGameObject(x, y), length(length), cellWidth(cellWidth), cellHeight(cellHeight) {
		this->isBlock = isBlock;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	int IsDirectionColliable(float nx, float ny);
	
};