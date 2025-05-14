#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"
#include "PlayHUD.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"
#include "FirePiranha.h"
#include "FireBullet.h"
#include "Koopas.h"

#include "Collision.h"
#include "LuckyBlock.h"
#include "UpgradeMarioLevel.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Update heldKoopas' position
	// move to here to optimize koopas' shell movement more smooth 
	// but can't be like real game 100%
	if (heldKoopas && canHold && isHolding)
	{
		PositionHeldKoopas(heldKoopas);
	}
	else if (heldKoopas && !canHold && isHolding)
	{
		isHolding = false;
		PositionHeldKoopas(heldKoopas);
		heldKoopas->setIsHeld(false);
		heldKoopas->setIsReleased(true);
		kickShell(heldKoopas);
		heldKoopas = nullptr;
	}
	vx += ax * dt;
	vy += ay * dt;
	if (abs(vx) > abs(maxVx))
	{
		vx = maxVx;
		//if (vx > 0)
		//	DebugOut(L" +reach max speed = %f\n", vx);
		//else
		//	DebugOut(L" -reach max speed= %f\n", vx);
	}

	// when releasing A key, and player is holding koopas, kick it 


	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::PositionHeldKoopas(LPGAMEOBJECT koopas)
{
	// if mario state != small, then the offset is different a bit.
	float offsetX, offsetY;
	if (level == MARIO_LEVEL_BIG)
	{
		offsetX = 11.0f;
		offsetY = -2.0f;
	}
	else if (level == MARIO_LEVEL_WITH_TAIL)
	{
		offsetX = 15.0f;
		offsetY = -2.0f;
	}
	else
	{
		offsetX = 11.0f;
		offsetY = 2.0f;
	}
	offsetX = (nx > 0 ? offsetX : -offsetX);
	koopas->SetPosition(x + offsetX, y - offsetY);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else
		if (e->nx != 0 && e->obj->IsBlocking())
		{
			vx = 0;
		}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CFirePiranha*>(e->obj))
		OnCollisionWithFirePiranha(e);
	else if (dynamic_cast<CFireBullet*>(e->obj))
	{
		OnCollisionWithFireBullet(e);
	}
	else if (dynamic_cast<CKoopas*>(e->obj))
		OnCollisionWithKoopas(e);
	else if (dynamic_cast<CLuckyBlock*>(e->obj))
		OnCollisionWithLuckyBlock(e);
	else if (dynamic_cast<CUpgradeLevel*>(e->obj))
		OnCollisionWithUpgradingItem(e);
}

void CMario::OnCollisionWithFirePiranha(LPCOLLISIONEVENT e)
{
	if (untouchable == 0)
	{
		if (level == MARIO_LEVEL_WITH_TAIL)
		{
			level = MARIO_LEVEL_BIG;
			StartUntouchable();
		}
		else if (level == MARIO_LEVEL_BIG)
		{
			level = MARIO_LEVEL_SMALL;
			StartUntouchable();
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}
	}
}

void CMario::OnCollisionWithFireBullet(LPCOLLISIONEVENT e)
{
	DebugOut(L"[INFO] Mario hit by bullet!\n");
	if (untouchable == 0)
	{
		if (level == MARIO_LEVEL_WITH_TAIL)
		{
			level = MARIO_LEVEL_BIG;
			StartUntouchable();
		}
		else if (level == MARIO_LEVEL_BIG)
		{
			level = MARIO_LEVEL_SMALL;
			StartUntouchable();
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}
	}

	e->obj->Delete();
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0 && goomba->GetState() != GOOMBA_STATE_DIE)
	{
		CRedGoomba* redGoomba = dynamic_cast<CRedGoomba*>(goomba);
		if (redGoomba && redGoomba->GetState() == GOOMBA_STATE_YES_WING)
		{
			redGoomba->SetState(GOOMBA_STATE_NO_WING);
		}
		else
		{
			goomba->SetState(GOOMBA_STATE_DIE);
		}
		vy = -MARIO_JUMP_DEFLECT_SPEED;
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level == MARIO_LEVEL_WITH_TAIL)
				{
					level = MARIO_LEVEL_BIG;
					StartUntouchable();
				}
				else if (level == MARIO_LEVEL_BIG)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::kickShell(CKoopas*& koopas)
{
	if (nx < 0)
		koopas->SetState(KOOPAS_STATE_SHELLIDLE_MOVING_LEFT);
	else
		koopas->SetState(KOOPAS_STATE_SHELLIDLE_MOVING_RIGHT);
	kick_flag = true;
	SetState(MARIO_STATE_KICK);
}
void CMario::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
	LPGAME game = CGame::GetInstance();
	if (e->ny > 0)
	{
		if (untouchable == 0)
		{
			if (level == MARIO_LEVEL_WITH_TAIL)
			{
				level = MARIO_LEVEL_BIG;
				StartUntouchable();
			}
			else if (level == MARIO_LEVEL_BIG)
			{
				level = MARIO_LEVEL_SMALL;
				StartUntouchable();
			}
			else
			{
				DebugOut(L">>> Mario DIE >>> \n");
				SetState(MARIO_STATE_DIE);
			}
		}
	}
	else if (e->ny < 0)
	{
		// if koopas is already in shell state, turn it be moving 
		if (koopas->GetState() == KOOPAS_STATE_SHELL || koopas->GetState() == KOOPAS_STATE_REGEN)
		{

			/*	ALERT !!!
					we dont use e->nx < 0 || e->nx > 0 because
					when we hit koopas with e->ny < 0 , most of the time,
					the e->nx will be == 0

				if (e->nx < 0)
					koopas->SetState(KOOPAS_STATE_SHELLIDLE_MOVING_LEFT);
				else if (e->nx > 0)
					koopas->SetState(KOOPAS_STATE_SHELLIDLE_MOVING_RIGHT);

				==> useless
			*/

			// we use the direct position of both objects to compare and set state 
			float x, y;
			koopas->GetPosition(x, y);

			//compare mario->x and koopas->x
			if (this->x < x)
				koopas->SetState(KOOPAS_STATE_SHELLIDLE_MOVING_RIGHT);
			else if (this->x >= x)
				koopas->SetState(KOOPAS_STATE_SHELLIDLE_MOVING_LEFT);

			vy = -MARIO_JUMP_DEFLECT_SPEED;

		}
		else if (koopas->GetState() != KOOPAS_STATE_SHELL)
		{
			// if koopas has wing, skrink the wing 
			// turn to normal walking one
			if (koopas->GetState() == KOOPAS_STATE_WING)
				koopas->SetState(KOOPAS_STATE_WALKING_LEFT);
			//from the walk one to the shell
			else
				koopas->SetState(KOOPAS_STATE_SHELL);
			//mario bounce back
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}

	}
	else if (e->nx != 0)
	{
		if (koopas->GetState() == KOOPAS_STATE_SHELL || koopas->GetState() == KOOPAS_STATE_REGEN)
		{
			// holding shell
			if (canHold && !isHolding)
			{
				isHolding = true;
				heldKoopas = koopas;
				heldKoopas->setIsHeld(true);
				heldKoopas->setIsReleased(false);
			}
			// let the hold and release A key in Update because when holding 
			// two obj are not collide anymore, so put them in update is good


			// kick it when not holding A key
			else if (!canHold && !isHolding)
			{
				kickShell(koopas);
			}
		}
		else
		{
			if (untouchable == 0)
			{
				if (level == MARIO_LEVEL_WITH_TAIL)
				{
					level = MARIO_LEVEL_BIG;
					StartUntouchable();
				}
				else if (level == MARIO_LEVEL_BIG)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}

	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
	CPlayHUD::GetInstance()->SetCoin(coin);;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithLuckyBlock(LPCOLLISIONEVENT e)
{
	CLuckyBlock* lb = dynamic_cast<CLuckyBlock*>(e->obj);
	if (e->ny > 0)
	{
		lb->setIsHit(true);
	}
}

void CMario::OnCollisionWithUpgradingItem(LPCOLLISIONEVENT e)
{
	if (e->nx != 0 || e->ny != 0)
	{
		if (dynamic_cast<CMushroom1UP*>(e->obj))
		{
			DebugOut(L"Life++\n");
			e->obj->Delete();
		}
		else if (level == 1 && dynamic_cast<CMushroomUpgradingMarioLevel*>(e->obj))
		{
			SetLevel(2);
			e->obj->Delete();
		}
		else if (level >= 2 && dynamic_cast<CLeaf*>(e->obj))
		{
			//if (level == 3) point += 1000;
			//else SetLevel(3);
			SetLevel(3);
			e->obj->Delete();
		}
	}
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;


	if (kick_flag)
	{
		// $$$$$ delay time for the kick animation 
		if (GetTickCount64() - kick_start < 200)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SMALL_KICK_RIGHT;
			else if (nx < 0)
				aniId = ID_ANI_MARIO_SMALL_KICK_LEFT;
		}
		else
			kick_flag = false;
	}
	else if (isHolding && canHold)
	{
		if (vx == 0)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SMALL_STANDING_HOLDSHELL_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_STANDING_HOLDSHELL_LEFT;
		}
		else
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SMALL_RUNNING_HOLDSHELL_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_RUNNING_HOLDSHELL_LEFT;
		}
	}
	else if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (kick_flag)
	{
		// $$$$$ delay time for the kick animation 
		if (GetTickCount64() - kick_start < 200)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_KICK_RIGHT;
			else if (nx < 0)
				aniId = ID_ANI_MARIO_KICK_LEFT;
		}
		else
			kick_flag = false;
	}
	else if (isHolding && canHold)
	{
		if (vx == 0)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_STANDING_HOLDSHELL_RIGHT;
			else
				aniId = ID_ANI_MARIO_STANDING_HOLDSHELL_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RUNNING_HOLDSHELL_RIGHT;
			else
				aniId = ID_ANI_MARIO_RUNNING_HOLDSHELL_LEFT;
		}
	}
	else if (!isOnPlatform)
	{
		// when reach max speed -> jump_run
		if (abs(vx) == MARIO_RUNNING_SPEED)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}
	if (aniId == -1)
	{
		if (nx >= 0)
			aniId = ID_ANI_MARIO_IDLE_RIGHT;
		else
			aniId = ID_ANI_MARIO_IDLE_LEFT;
	}
	return aniId;
}
int CMario::GetAniIdWithTail()
{
	int aniId = -1;
	if (kick_flag)
	{
		// $$$$$ delay time for the kick animation 
		if (GetTickCount64() - kick_start < 200)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_WITH_TAIL_KICK_RIGHT;
			else if (nx < 0)
				aniId = ID_ANI_MARIO_WITH_TAIL_KICK_LEFT;
		}
		else
			kick_flag = false;
	}
	else if (isHolding && canHold)
	{
		if (vy != 0)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_WITH_TAIL_JUMP_HOLDSHELL_RIGHT;
			else
				aniId = ID_ANI_MARIO_WITH_TAIL_JUMP_HOLDSHELL_LEFT;
		}
		else
		{
			if (vx == 0)
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_WITH_TAIL_STANDING_HOLDSHELL_RIGHT;
				else
					aniId = ID_ANI_MARIO_WITH_TAIL_STANDING_HOLDSHELL_LEFT;
			}
			else
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_WITH_TAIL_RUNNING_HOLDSHELL_RIGHT;
				else
					aniId = ID_ANI_MARIO_WITH_TAIL_RUNNING_HOLDSHELL_LEFT;
			}
		}

	}
	else if (!isOnPlatform)
	{
		/*
			animation for tail is seperated into 2 part:
			+ first is flying up : the tail is "normal" : paralel with the ground
			+ second is dropping down: the tail resisted by air resistance
					-> make it flies up a bit : bend as a parabol

		*/
		if (abs(vx) == MARIO_RUNNING_SPEED)
		{
			if (vy < 0)
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_WITH_TAIL_JUMP_RUN_RIGHT;
				else
					aniId = ID_ANI_MARIO_WITH_TAIL_JUMP_RUN_LEFT;
			}
			// drop
			else
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_WITH_TAIL_JUMP_RUN_RIGHT_RELEASE;
				else
					aniId = ID_ANI_MARIO_WITH_TAIL_JUMP_RUN_LEFT_RELEASE;
			}

		}
		else
		{
			// fly
			if (vy < 0)
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_WITH_TAIL_JUMP_WALK_RIGHT;
				else
					aniId = ID_ANI_MARIO_WITH_TAIL_JUMP_WALK_LEFT;
			}
			// drop
			else
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_WITH_TAIL_JUMP_WALK_RIGHT_RELEASE;
				else
					aniId = ID_ANI_MARIO_WITH_TAIL_JUMP_WALK_LEFT_RELEASE;
			}
		}
	}
	else if (isSitting)
	{
		if (nx > 0)
			aniId = ID_ANI_MARIO_WITH_TAIL_SIT_RIGHT;
		else
			aniId = ID_ANI_MARIO_WITH_TAIL_SIT_LEFT;
	}
	else if (vx == 0)
	{
		if (nx > 0)
			aniId = ID_ANI_MARIO_WITH_TAIL_IDLE_RIGHT;
		else
			aniId = ID_ANI_MARIO_WITH_TAIL_IDLE_LEFT;
	}
	else if (vx > 0)
	{
		if (ax < 0)
			aniId = ID_ANI_MARIO_WITH_TAIL_BRACE_RIGHT;
		else if (ax == MARIO_ACCEL_RUN_X)
			aniId = ID_ANI_MARIO_WITH_TAIL_RUNNING_RIGHT;
		else if (ax == MARIO_ACCEL_WALK_X)
			aniId = ID_ANI_MARIO_WITH_TAIL_WALKING_RIGHT;
	}
	else // vx < 0
	{
		if (ax > 0)
			aniId = ID_ANI_MARIO_WITH_TAIL_BRACE_LEFT;
		else if (ax == -MARIO_ACCEL_RUN_X)
			aniId = ID_ANI_MARIO_WITH_TAIL_RUNNING_LEFT;
		else if (ax == -MARIO_ACCEL_WALK_X)
			aniId = ID_ANI_MARIO_WITH_TAIL_WALKING_LEFT;
	}

	if (aniId == -1)
	{
		if (nx >= 0)
			aniId = ID_ANI_MARIO_WITH_TAIL_IDLE_RIGHT;
		else
			aniId = ID_ANI_MARIO_WITH_TAIL_IDLE_LEFT;
	}
	return aniId;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_WITH_TAIL)
		aniId = GetAniIdWithTail();
	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();

	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y += MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	case MARIO_STATE_KICK:
		kick_start = GetTickCount64();
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_WITH_TAIL)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH / 2;
		top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

