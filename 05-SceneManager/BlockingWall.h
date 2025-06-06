#pragma once
#include "GameObject.h"
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16
class CBlockingWall :
	public CGameObject
{
public:
	CBlockingWall(float x, float y) : CGameObject(x, y) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 1; }
	void Render() { RenderBoundingBox(); }
};

