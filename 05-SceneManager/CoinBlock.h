#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN_BLOCK 7000
#define ID_ANI_BRICK_BLOCK 7001

#define COINBLOCK_WIDTH 16
#define COINBLOCK_BBOX_HEIGHT 16
#define COINBLOCK_BBOX_WIDTH 16

enum CoinBlockState {
    COIN_BLOCK_STATE_COIN,
    COIN_BLOCK_STATE_BRICK
};

class CCoinBlock : public CGameObject
{
protected:
    int state;
    bool isConverted; 

public:
    CCoinBlock(float x, float y, int initState = COIN_BLOCK_STATE_BRICK);

    void SetState(int state);
    int GetState() { return state; }

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();

    void GetBoundingBox(float& l, float& t, float& r, float& b);
};
