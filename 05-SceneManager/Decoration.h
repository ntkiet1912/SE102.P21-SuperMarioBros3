#pragma once
#include "GameObject.h"
#include "vector"   

class CDecoration : public CGameObject
{
protected:
	std::vector<std::pair<float, float>> positions;
	std::vector<int> sprites;
public:
	CDecoration(float x, float y, const std::vector<std::pair<float, float>>& positions, const std::vector<int>& sprites)
		: CGameObject(x, y), positions(positions), sprites(sprites) {}
	void Render();
	void Update(DWORD dt){}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {}
	void RenderBoundingBox() {}
};