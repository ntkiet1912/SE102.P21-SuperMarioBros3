#pragma once
#pragma once

#include "GameObject.h"

#define PIPE_BORDER 2
#define PIPE_CELL_WIDTH 32
#define PIPE_CELL_HEIGHT 16
#define PIPE_HEAD_ID 800000
#define PIPE_BODY_ID 800001
class CBigPipe : public CGameObject
{
private:
	int height;
	int headId;
	int bodyId;
public:

	CBigPipe(float x, float y, int height) :CGameObject(x, y)
	{
		this->height = height;
		headId = PIPE_HEAD_ID;
		bodyId = PIPE_BODY_ID;
	}
	CBigPipe(float x, float y, int height, int headId, int bodyId) :CGameObject(x, y) {
		this->height = height;
		this->headId = headId;
		this->bodyId = bodyId;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	int IsCollidable() { return 1; }
};

typedef CBigPipe* LPBigPipe;