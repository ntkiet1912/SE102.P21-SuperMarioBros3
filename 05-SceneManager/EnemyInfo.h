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

    int length = 0;
    float cellWidth = 0;
    float cellHeight = 0;
    int spriteIdBegin = 0, spriteIdMiddle = 0, spriteIdEnd = 0;

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
    CEnemySpawnInfo(int type, float x, float y,
        float cell_width, float cell_height, int length,
        int sprite_id_begin, int sprite_id_middle, int sprite_id_end)
    {
        this->x = x;
        this->y = y;
        this->type = type;
        this->length = length;
        this->cellWidth = cell_width;
        this->cellHeight = cell_height;
        this->spriteIdBegin = sprite_id_begin;
        this->spriteIdMiddle = sprite_id_middle;
        this->spriteIdEnd = sprite_id_end;
        this->isSpawned = false;
        this->instance = nullptr;
    }

};

