﻿#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetState() == MARIO_ENDING_SCENE) return;

	switch (KeyCode)
	{
	case DIK_RIGHT:
	case DIK_LEFT:
		mario->setCanSit(false);
		break;
	case DIK_DOWN:

		// make sure that when holding shell
		// big mario can't sit -> lead to wrong logic
		if (!mario->getIsHolding() && mario->getCanSit())
		{
			mario->SetState(MARIO_STATE_SIT);
		}
		break;
	case DIK_S:
		mario->SetState(MARIO_STATE_JUMP);
		mario->canFly = true;
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	case DIK_F3:
		mario->SetPosition(2279, 80);
		break;
	case DIK_R: // reset
		//Reload();
		break;

	case DIK_A:
		mario->setCanHold(true);
		if (mario->getLevel() == 3 && !mario->getIsSitting())
		{
			mario->SetState(MARIO_STATE_TAIL_ATTACK);
		}
		break;

	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetState() == MARIO_ENDING_SCENE) return;
	switch (KeyCode)
	{
	case DIK_S:
		mario->SetState(MARIO_STATE_RELEASE_JUMP);
		mario->canFly = false;
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	case DIK_A:
		mario->setCanHold(false);
		break;
	case DIK_RIGHT:
	case DIK_LEFT:
		mario->setCanSit(true);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE* states)
{
	if (((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer() == NULL) return;
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (!mario->getIsActive())
	{
		return;
	}
	if (mario->GetState() == MARIO_ENDING_SCENE) return;
	if (game->IsKeyDown(DIK_UP)) {
			mario->isAbleToRise = true;
	}
	else mario->isAbleToRise = false;
	if (game->IsKeyDown(DIK_DOWN)) {
			mario->isAbleToDive = true;
	}
	else mario->isAbleToDive = false;
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A))
		{
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		}
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A))
		{
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		}
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else
	{
		if (mario != nullptr) {
			mario->SetState(MARIO_STATE_IDLE);
			mario->setCanSit(true);
		}
		else {
			OutputDebugString(L"mario is nullptr!\n");
		}
	}
}