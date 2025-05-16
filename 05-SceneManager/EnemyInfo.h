#pragma once
#include "Collision.h"
class CEnemySpawnInfo
{
public:
    int type;
    float x, y;
    bool yesWing;
    bool isRed;
    bool isSpawned;
    LPGAMEOBJECT instance;
    CEnemySpawnInfo(int type, float x, float y, bool isRed = false, bool yesWing = false)
    {
        this->type = type;
        this->x = x;
        this->y = y;
        this->isRed = isRed;
        this->yesWing = yesWing;
        this->isSpawned = false;
        this->instance = nullptr;
    }
};

