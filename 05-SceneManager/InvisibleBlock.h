#pragma once
#include "GameObject.h"

class CInvisibleBlock : public CGameObject
{
protected:
	int length;
	float cellWidth;
	float cellHeight;
public:
	CInvisibleBlock(float x, float y, int length, float cellWidth, float cellHeight)
		: CGameObject(x, y), length(length), cellWidth(cellWidth), cellHeight(cellHeight) {
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	int IsDirectionColliable(float nx, float ny);
	
};