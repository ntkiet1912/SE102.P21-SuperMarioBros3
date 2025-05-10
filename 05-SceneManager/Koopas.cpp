#include "Koopas.h"
#include "Debug.h"
#include "Brick.h"
#include "Goomba.h"
#include "Portal.h"
#include "Platform.h"
#include "Coin.h"
#include "Game.h"
#include "PlayScene.h"
#include "LuckyBlock.h"
#include "InvisibleBlock.h"



CKoopas::CKoopas(float x, float y, int isRed, int yesWing) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPAS_GRAVITY;
	die_start = 0;
	this->isRed = isRed;
	this->yesWing = yesWing;
	isFlyingUp = false;
	isShellIdle = false;
	isRegen = false;
	if (yesWing)
		SetState(KOOPAS_STATE_WING);
	else
		SetState(KOOPAS_STATE_WALKING_LEFT);
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isShellIdle)
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_SHELL_HEIGHT / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = y + KOOPAS_BBOX_SHELL_HEIGHT / 2;
	}
	// when die, no bounding box for unexpected error
	else if (die_start)
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}

	// normal state
	else
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT;
	}

}

void CKoopas::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
	}

	// collide with block like brick or lucky box
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		if (state == KOOPAS_STATE_WALKING_RIGHT)
			SetState(KOOPAS_STATE_WALKING_LEFT);
		else if (state == KOOPAS_STATE_SHELLIDLE_MOVING_RIGHT)
			SetState(KOOPAS_STATE_SHELLIDLE_MOVING_LEFT);
		else if (state == KOOPAS_STATE_WALKING_LEFT)
			SetState(KOOPAS_STATE_WALKING_RIGHT);
		else if (state == KOOPAS_STATE_SHELLIDLE_MOVING_LEFT)
			SetState(KOOPAS_STATE_SHELLIDLE_MOVING_RIGHT);
	}

	if (dynamic_cast<CCoin*>(e->obj)) return;
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	//else if (dynamic_cast<CPlatform*>(e->obj))
	//	OnCollisionWithPlatform(e);
	else if ((dynamic_cast<CKoopas*>(e->obj)))
		OnCollisionWithKoopas(e);
	else if (dynamic_cast<CLuckyBlock*>(e->obj))
		OnCollisionWithLuckyBlock(e);

}
//void CKoopas::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
//{
//	CPlatform* platform = dynamic_cast<CPlatform*>(e->obj);
//	if (isRed)
//	{
//		// platform rounding box
//		float top, left, bottom, right;
//		platform->GetBoundingBox(left, top, right, bottom);
//		// position of Koopas
//		float x, y;
//		this->GetPosition(x, y);
//
//		if (e->ny < 0) // on platform
//		{
//			// if reached the bound of platform
//			if (x + KOOPAS_BBOX_WIDTH / 2 > right)
//			{
//				if (state == KOOPAS_STATE_WALKING_RIGHT)
//					SetState(KOOPAS_STATE_WALKING_LEFT);
//			}
//			else if (x - KOOPAS_BBOX_WIDTH / 2 < left)
//			{
//				if (state == KOOPAS_STATE_WALKING_LEFT)
//					SetState(KOOPAS_STATE_WALKING_RIGHT);
//			}
//		}
//	}
//}

void CKoopas::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	if (!isShellIdle) return;
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (state == KOOPAS_STATE_SHELLIDLE_MOVING_LEFT || state == KOOPAS_STATE_SHELLIDLE_MOVING_RIGHT)
	{
		if (e->nx != 0)
			goomba->SetState(GOOMBA_STATE_DIE_BY_COLLISION);
	}
}
void CKoopas::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	if (!isShellIdle) return;
	else
	{
		CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
		if (e->nx != 0 || e->ny != 0)
		{
			if (state == KOOPAS_STATE_SHELLIDLE_MOVING_LEFT || state == KOOPAS_STATE_SHELLIDLE_MOVING_RIGHT)
			{
				koopas->SetState(KOOPAS_STATE_SHELL_DIE_BY_COLLISION_WITH_KOOPAS);
				if (e->nx > 0)
					koopas->vx = -KOOPAS_VX_DIE_SPEED;
				else
					koopas->vx = KOOPAS_VX_DIE_SPEED;

			}
		}

	}
}

void CKoopas::OnCollisionWithLuckyBlock(LPCOLLISIONEVENT e)
{
	CLuckyBlock* lb = dynamic_cast<CLuckyBlock*>(e->obj);
	if (state == KOOPAS_STATE_SHELLIDLE_MOVING_LEFT || state == KOOPAS_STATE_SHELLIDLE_MOVING_RIGHT)
	{
		if (e->nx != 0)
		{
			lb->setIsHit(true);
		}
	}
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	// red Koopas is more intelligent than the green one and only move on blocks
	// when outta block, he will "turn back"
	if (isRed && (state == KOOPAS_STATE_WALKING_LEFT || state == KOOPAS_STATE_WALKING_RIGHT))
	{

		if (!IsTherePlatformAhead(coObjects))
		{
			if (state == KOOPAS_STATE_WALKING_LEFT)
				SetState(KOOPAS_STATE_WALKING_RIGHT);
			else
				SetState(KOOPAS_STATE_WALKING_LEFT);
		}
	}
	if (state == KOOPAS_STATE_WING)
	{
		if (!isFlyingUp)
		{
			vy = -KOOPAS_FLYING_SPEED;
			isFlyingUp = true;
			flying_start = GetTickCount64();
		}
		else
		{
			// ready for next flying phase
			if (GetTickCount64() - flying_start > 1000)
			{
				vy = 0;
				isFlyingUp = false;
			}
		}
		ay = KOOPAS_GRAVITY_FLYING;
	}

	// (*) when is held, set the ay = 0 to make sure that they are not dropping while holding
	if (isHeld && (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_REGEN))
	{
		ay = 0;
		vx = 0;
		vy = 0;
	}
	// from the (*). when releasing, give the ay back.
	else if (isReleased)
	{
		isReleased = false;
		ay = KOOPAS_GRAVITY_FLYING;
	}
	// die by collide with koopas shell moving
	if (((state == KOOPAS_STATE_WALK_DIE_BY_COLLISION_WITH_KOOPAS) ||
		(state == KOOPAS_STATE_SHELL_DIE_BY_COLLISION_WITH_KOOPAS)) &&
		(GetTickCount64() - die_start > 1500))
	{
		isDeleted = true;
		return;
	}
	// first stage: from shell with no legs to shell with legs 
	if (state == KOOPAS_STATE_SHELL && GetTickCount64() - regen_start > 5000)
	{
		SetState(KOOPAS_STATE_REGEN);
		ay = KOOPAS_GRAVITY;
	}

	// second stage: from shell with legs to walking state 

	if (state == KOOPAS_STATE_REGEN && GetTickCount64() - realRegen_start > 1000)
	{
		// avoiding koopa to drop out of game
		// like mario when changing level
		y -= (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_SHELL_HEIGHT + 1) / 2;
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

		if (mario->getX() < x)
			SetState(KOOPAS_STATE_WALKING_LEFT);
		else
			SetState(KOOPAS_STATE_WALKING_RIGHT);

		// and if player is holding the shell while it's on real Regen time
		// when the koopas stay back to the walking state, player will get dmg
		if (mario->getIsHolding())
		{
			if (mario->getLevel() == MARIO_LEVEL_WITH_TAIL)
			{
				mario->SetLevel(MARIO_LEVEL_BIG);
				mario->StartUntouchable();
			}
			else if (mario->getLevel() == MARIO_LEVEL_BIG)
			{
				mario->SetLevel(MARIO_LEVEL_SMALL);
				mario->StartUntouchable();
			}
			else
			{
				mario->SetState(MARIO_STATE_DIE);
			}
		}
		mario->setIsHolding(false);
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopas::Render()
{
	int aniId = -1;
	// shell state
	if (state == KOOPAS_STATE_SHELL)
	{

		if (isRed)
			if (isRegen)

				aniId = ID_ANI_RED_KOOPAS_SHELL_REGEN;
			else
				aniId = ID_ANI_RED_KOOPAS_SHELL;
		else
			if (isRegen)
				aniId = ID_ANI_GREEN_KOOPAS_SHELL_REGEN;
			else
				aniId = ID_ANI_GREEN_KOOPAS_SHELL;
	}

	// shell moving state 
	if (state == KOOPAS_STATE_SHELLIDLE_MOVING_LEFT || state == KOOPAS_STATE_SHELLIDLE_MOVING_RIGHT)
	{
		if (isRed)
			aniId = ID_ANI_RED_KOOPAS_SHELL_MOVING;
		else
			aniId = ID_ANI_GREEN_KOOPAS_SHELL_MOVING;
	}

	// left 
	if (state == KOOPAS_STATE_WALKING_LEFT)
	{
		if (isRed)
			aniId = ID_ANI_RED_KOOPAS_WALKING_LEFT;
		else
			aniId = ID_ANI_GREEN_KOOPAS_WALKING_LEFT;
	}

	// right
	if (state == KOOPAS_STATE_WALKING_RIGHT)
	{
		if (isRed)
			aniId = ID_ANI_RED_KOOPAS_WALKING_RIGHT;
		else
			aniId = ID_ANI_GREEN_KOOPAS_WALKING_RIGHT;
	}

	//wing
	if (state == KOOPAS_STATE_WING)
	{
		if (isRed)
			aniId = ID_ANI_RED_KOOPAS_WING;
		else
			aniId = ID_ANI_GREEN_KOOPAS_WING;
	}

	//if (state == KOOPAS_STATE_WALK_DIE_BY_COLLISION_WITH_KOOPAS)
	//{
	//	//DebugOut(L"ok");
	//	if (isRed)
	//		aniId = ID_ANI_RED_KOOPAS_WALK_DIE_BY_COLLISION;
	//	else
	//		aniId = ID_ANI_GREEN_KOOPAS_WALK_DIE_BY_COLLISION;
	//}

	if (state == KOOPAS_STATE_SHELL_DIE_BY_COLLISION_WITH_KOOPAS)
	{
		if (isRed)
			aniId = ID_ANI_RED_KOOPAS_SHELL_DIE_BY_COLLISION;
		else
			aniId = ID_ANI_GREEN_KOOPAS_SHELL_DIE_BY_COLLISION;
	}
	if (state == KOOPAS_STATE_REGEN)
	{
		if (isRed)
			aniId = ID_ANI_RED_KOOPAS_SHELL_REGEN;
		else
			aniId = ID_ANI_GREEN_KOOPAS_SHELL_REGEN;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_SHELL:
		regen_start = GetTickCount64();	// time to go to phase 2: leg appear
		vx = 0;
		vy = 0;
		isShellIdle = true;
		isHeld = false;
		isReleased = false;
		ay = KOOPAS_GRAVITY;
		//ay = 0;
		break;
	case KOOPAS_STATE_REGEN:
		vx = 0;
		vy = 0;
		ay = KOOPAS_GRAVITY;

		isShellIdle = true;
		realRegen_start = GetTickCount64(); // time to go to phase 3: Koopas reborn
		break;

	case KOOPAS_STATE_WALKING_LEFT:
		vx = -KOOPAS_WALKING_SPEED;
		isShellIdle = false;
		ay = KOOPAS_GRAVITY;
		break;

	case KOOPAS_STATE_WALKING_RIGHT:
		vx = KOOPAS_WALKING_SPEED;
		isShellIdle = false;
		ay = KOOPAS_GRAVITY;

		break;

	case KOOPAS_STATE_SHELLIDLE_MOVING_RIGHT:
		vx = KOOPAS_SHELL_SPEED;
		isShellIdle = true;

		ay = KOOPAS_GRAVITY;
		break;

	case KOOPAS_STATE_SHELLIDLE_MOVING_LEFT:
		vx = -KOOPAS_SHELL_SPEED;
		isShellIdle = true;

		ay = KOOPAS_GRAVITY;
		break;

	case KOOPAS_STATE_WING:

		vx = -KOOPAS_WALKING_SPEED;
		flying_start = GetTickCount64();
		isShellIdle = false;
		break;

		//case KOOPAS_STATE_WALK_DIE_BY_COLLISION_WITH_KOOPAS:
	case KOOPAS_STATE_SHELL_DIE_BY_COLLISION_WITH_KOOPAS:
		die_start = GetTickCount64();
		vy = -KOOPAS_DYING_SPEED;
		ay = KOOPAS_GRAVITY_DYING;
		break;
	}
}

bool CKoopas::IsTherePlatformAhead(vector<LPGAMEOBJECT>* coObjects)
{
	// use 2 variables to check Koopas' next step is on platform or not 
	float checkX, checkY;
	if (vx > 0)
		// check to the right 
		checkX = x + KOOPAS_BBOX_WIDTH / 8;
	else
		// to the left 
		checkX = x - KOOPAS_BBOX_WIDTH / 8;
	// check under koopas' feet 
	checkY = y + KOOPAS_BBOX_HEIGHT / 2 + 2;

	// scan all the objects to find koopa isOnplatform
	for (LPGAMEOBJECT obj : *coObjects)
	{
		if (dynamic_cast<CBrick*>(obj) || dynamic_cast<CLuckyBlock*>(obj) || dynamic_cast<CInvisibleBlock*>(obj))
		{
			float l, t, r, b;
			obj->GetBoundingBox(l, t, r, b);

			// checking that is there a block under koopas' feet ? 
			if (checkX >= l && checkX <= r && checkY >= t && checkY <= b)
				return true;
		}
	}
	return false;
}