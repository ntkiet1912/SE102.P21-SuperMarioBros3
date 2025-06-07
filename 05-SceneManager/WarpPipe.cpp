#include "WarpPipe.h"
#include "Game.h"
#include "PlayScene.h"
#include "Portal.h"


void CWarpPipe::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isWarping) return;
	warpTime -= dt;
	if (warpTime < 0) {
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		mario->SetState(MARIO_STATE_RELEASE_JUMP);

		if (state == PIPE_STATE_DOWN) {
			int nextSceneid = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetNextSceneID();
			int currentID = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->getID();

			if (nextSceneid == 4 && currentID == 4) {
				CGame::GetInstance()->InitiateSwitchScene(8);
				mario->SetPosition(2136, 20);

			}
			else {
				CGame::GetInstance()->InitiateSwitchScene(7);
				mario->SetPosition(143, 0);
			}
			
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
