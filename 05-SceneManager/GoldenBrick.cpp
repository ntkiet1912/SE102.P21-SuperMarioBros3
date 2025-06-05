#include "GoldenBrick.h"
#include "PlayScene.h"
#include "GameManager.h"

void CGoldenBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == GOLDEN_BRICK_STATE_NORMAL) return;
	ULONGLONG deltaTime = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetDeltaTime(state_start);
	if (state == GOLDEN_BRICK_STATE_GOLD
		&& deltaTime > GOLDEN_BRICK_STATE_GOLD_TIMEOUT)
		SetState(GOLDEN_BRICK_STATE_NORMAL);
	y += vy * dt;
	if (state == GOLDEN_BRICK_STATE_MOVE_UP)
	{
		if (y <= minY)
		{
			y = minY;
			SetState(GOLDEN_BRICK_STATE_MOVE_DOWN);
		}
	}
	else if (state == GOLDEN_BRICK_STATE_MOVE_DOWN)
	{
		if (y >= maxY)
		{
			y = maxY;
			SetState(GOLDEN_BRICK_STATE_NORMAL);
		}
	}
}

void CGoldenBrick::Render()
{
	int aniId = ID_ANI_GOLDEN_BRICK_NORMAL;
	if (state == GOLDEN_BRICK_STATE_GOLD) aniId = ID_ANI_GOLDEN_BRICK_GOLD;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

int CGoldenBrick::IsBlocking()
{
	if (state == GOLDEN_BRICK_STATE_NORMAL) return 1;
	return 0;
}


void CGoldenBrick::HitByMario()
{
	SetState(GOLDEN_BRICK_STATE_MOVE_UP);
}

void CGoldenBrick::Break()
{
	CGameManager::GetInstance()->AddBrokenEffect(x, y);
	Delete();
}

void CGoldenBrick::SetState(int _state)
{
	this->state = _state;
	switch (this->state)
	{
	case GOLDEN_BRICK_STATE_NORMAL:
		vy = 0;
		break;
	case GOLDEN_BRICK_STATE_GOLD:
		state_start = GetTickCount64();
		break;
	case GOLDEN_BRICK_STATE_MOVE_UP:
		vy = -GOLDEN_BRICK_MOVE_SPEED;
		break;
	case GOLDEN_BRICK_STATE_MOVE_DOWN:
		vy = GOLDEN_BRICK_MOVE_SPEED;
		break;

	}
}
