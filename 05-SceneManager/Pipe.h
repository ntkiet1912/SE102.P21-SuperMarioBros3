#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include <vector>

class CPipe : public CGameObject
{
protected:
	std::vector<std::pair<float, float>> positions;
	std::vector<int> sprites;
	int length;
	float cellWidth;
	float cellHeight;
public:
	CPipe(float x, float y, int length, float cellWidth, float cellHeight, const std::vector<std::pair<float, float>>& positions, const std::vector<int>& sprites)
		:CGameObject(x, y), positions(positions), sprites(sprites) {
		this->length = length;
		this->cellWidth = cellWidth;
		this->cellHeight = cellHeight;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);	
	void RenderBoundingBox();
};