#include "ButtonBrick.h"
#include "Game.h"
#include "PlayScene.h"

void CButtonBrick::Render()
{
	int aniId = ID_ANI_BUTTON_BRICK_HIT;
	if (state == BUTTON_BRICK_STATE_NORMAL) aniId = ID_ANI_BUTTON_BRICK_NORMAL;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CButtonBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == BUTTON_BRICK_STATE_NORMAL || state == BUTTON_BRICK_STATE_HIT) return;
	y += vy * dt;
	if (state == BUTTON_BRICK_STATE_MOVE_UP)
	{
		if (y <= minY)
		{
			y = minY;
			SetState(BUTTON_BRICK_STATE_MOVE_DOWN);
		}
	}
	else if (state == BUTTON_BRICK_STATE_MOVE_DOWN)
	{
		if (y >= maxY)
		{
			y = maxY;
			SetState(BUTTON_BRICK_STATE_HIT);
		}
	}
	if (button) button->SetPosition(x, y - BRICK_BBOX_HEIGHT);
}

void CButtonBrick::SetState(int _state)
{
	state = _state;
	switch (state)
	{
	case BUTTON_BRICK_STATE_NORMAL:
		vy = 0;
		break;
	case BUTTON_BRICK_STATE_HIT:
		vy = 0;
		break;
	case BUTTON_BRICK_STATE_MOVE_UP:
		button = new CButton(x, y - BRICK_BBOX_HEIGHT);
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddObject(button);
		vy = -BUTTON_BRICK_MOVE_SPEED;
		break;
	case BUTTON_BRICK_STATE_MOVE_DOWN:
		vy = BUTTON_BRICK_MOVE_SPEED;
		break;
	}

}
