#include "WarpPipe.h"
#include "Game.h"
#include "PlayScene.h"


void CWarpPipe::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isWarping) return;
	warpTime -= dt;
	if (warpTime < 0) {
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		mario->SetState(MARIO_STATE_RELEASE_JUMP);

		if (state == PIPE_STATE_DOWN) {
			CGame::GetInstance()->InitiateSwitchScene(7);
			mario->SetPosition(143, 0);
		}
		else {
			CGame::GetInstance()->InitiateSwitchScene(6);
			mario->SetPosition(2480, 162);

		}
	}
}

void CWarpPipe::HandleWithMario(LPCOLLISIONEVENT e, CMario* mario)
{
	if (state == PIPE_STATE_DOWN) {
		if (e->ny < 0 && mario->isAbleToDive) {
			isWarping = true;
			mario->Warp(0.05f, 1500);
			warpTime = WARP_TIME;
		}
	}
	else {
		if (e->ny > 0 && mario->isAbleToRise) {
			isWarping = true;
			mario->Warp(-0.05f, 1000);
			warpTime = WARP_TIME;
		}
	}
}
