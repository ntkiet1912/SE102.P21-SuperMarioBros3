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
#include "Block.h"
#include "DataManager.h"
#include "DeadZone.h"
#include "GameManager.h"

CKoopas::CKoopas(float x, float y, int isRed, int yesWing) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPAS_GRAVITY;
	die_start = 0;
	this->isRed = isRed;
	this->yesWing = yesWing;
	isRegen = false;
	isUpsideDown = false;
	isOnGround = false;
	if (yesWing)
		SetState(KOOPAS_STATE_WING);
	else
		SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPAS_STATE_WALKING || state == KOOPAS_STATE_WING)
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT;
	}
	else if (die_start > 0)
	{
		left = right = top = bottom = 0; // no bounding box when die
	}
	else
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_SHELL_HEIGHT / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_SHELL_HEIGHT;
	}

}

void CKoopas::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;

};


void CKoopas::GetStomped() {
	CGameManager::GetInstance()->StomKoopa(x, y - KOOPAS_BBOX_HEIGHT);
}

void CKoopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (state == KOOPAS_STATE_WING)
	{
		// only green one bounces - red one just flies up and down
		if(!isRed)
		{
			if (e->ny != 0 && e->obj->IsBlocking())
			{
				vy = -KOOPAS_FLYING_SPEED_VY;
			}
			else
			{
				vy = 0;
			}
			ay = KOOPAS_GRAVITY_FLYING;
		}
	}
	else if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		isOnGround = true;
	}
	// collide with block like brick or lucky box
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}


	if (dynamic_cast<CCoin*>(e->obj)) return;

	if (state == KOOPAS_STATE_HELD || state == KOOPAS_STATE_SHELL_MOVING)
	{
		if (dynamic_cast<CGoomba*>(e->obj))
			OnCollisionWithGoomba(e);
		else if ((dynamic_cast<CKoopas*>(e->obj)))
			OnCollisionWithKoopas(e);
		else if (dynamic_cast<CLuckyBlock*>(e->obj))
			OnCollisionWithLuckyBlock(e);
	}
}

void CKoopas::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (e->nx != 0) {
		goomba->SetState(GOOMBA_STATE_DIE_BY_COLLISION);
		CDataManager::GetInstance()->AddScore(1000);
		goomba->GetStomped();
	}
}

void CKoopas::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
	if (e->nx != 0 || e->ny != 0)
	{
		koopas->SetState(KOOPAS_STATE_DIE);
		CDataManager::GetInstance()->AddScore(1000);
		koopas->GetStomped();
		if (e->nx > 0)
			koopas->vx = -KOOPAS_VX_DIE_SPEED;
		else
			koopas->vx = KOOPAS_VX_DIE_SPEED;

	}
}


void CKoopas::OnCollisionWithLuckyBlock(LPCOLLISIONEVENT e)
{
	CLuckyBlock* lb = dynamic_cast<CLuckyBlock*>(e->obj);
	if (e->nx != 0)
	{
		lb->setIsHit(true);
	}

}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += ax * dt;
	vy += ay * dt;
	// red Koopas is more intelligent than the green one and only move on blocks
	// when outta block, he will "turn back"
	if (isRed && (state == KOOPAS_STATE_WALKING) && isOnGround)
	{
		if (!IsTherePlatformAhead(coObjects))
		{
			vx = -vx;
		}
	}
	if (isRed && yesWing && state == KOOPAS_STATE_WING)
	{
		if (y < 16)
		{
			vy = RED_KOOPAS_FLYING_SPEED_VY;
		}
		else if (y > CGame::GetInstance()->GetBackBufferHeight() - 50)
		{
			vy = -RED_KOOPAS_FLYING_SPEED_VY;
		}
	}
	if (state == KOOPAS_STATE_SHELL_UPSIDE_DOWN && isOnGround)
	{
		SetState(KOOPAS_STATE_SHELL);
	}
	// die by collide with koopas shell moving
	if ((state == KOOPAS_STATE_DIE) &&
		(GetTickCount64() - die_start > 1500))
	{
		isDeleted = true;
		return;
	}

	// first stage: from shell with no legs to shell with legs 
	if ((state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_HELD) && GetTickCount64() - regen_start > 5000)
	{
		SetState(KOOPAS_STATE_REGEN);
		//ay = KOOPAS_GRAVITY;
	}

	// second stage: from shell with legs to walking state 
	if (state == KOOPAS_STATE_REGEN && GetTickCount64() - realRegen_start > 1000)
	{
		// avoiding koopa to drop out of game
		// like mario when changing level
		isUpsideDown = false;
		y -= (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_SHELL_HEIGHT + 1) / 2;
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

		SetState(KOOPAS_STATE_WALKING);
		// and if player is holding the shell while it's on real Regen time
		// when the koopas stay back to the walking state, player will get dmg
		if (mario->getIsHolding())
		{
			mario->getDmg();
		}
		mario->setIsHolding(false);
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopas::Render()
{
	int aniId = -1;
	if (state == KOOPAS_STATE_SHELL_UPSIDE_DOWN)
	{
		if (isRed)
			aniId = ID_ANI_RED_KOOPAS_SHELL_USD;
		else
			aniId = ID_ANI_GREEN_KOOPAS_SHELL_USD;
	}
	// shell state
	if (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_HELD)
	{
		if (isUpsideDown)
		{
			if (isRed)
				aniId = ID_ANI_RED_KOOPAS_SHELL_USD;
			else
				aniId = ID_ANI_GREEN_KOOPAS_SHELL_USD;
		}
		else
		{
			if (isRed)
				aniId = ID_ANI_RED_KOOPAS_SHELL;
			else
				aniId = ID_ANI_GREEN_KOOPAS_SHELL;
		}
	}

	// shell moving state 
	if (state == KOOPAS_STATE_SHELL_MOVING)
	{
		if (isUpsideDown)
		{
			if (isRed)
				aniId = ID_ANI_RED_KOOPAS_SHELL_MOVING_USD;
			else
				aniId = ID_ANI_GREEN_KOOPAS_SHELL_MOVING_USD;
		}
		else
		{
			if (isRed)
				aniId = ID_ANI_RED_KOOPAS_SHELL_MOVING;
			else
				aniId = ID_ANI_GREEN_KOOPAS_SHELL_MOVING;
		}
	}

	if (state == KOOPAS_STATE_WALKING)
	{
		if (isRed)
		{
			if (vx > 0)
			{
				aniId = ID_ANI_RED_KOOPAS_WALKING_RIGHT;
			}
			else
			{
				aniId = ID_ANI_RED_KOOPAS_WALKING_LEFT;
			}
		}
		else
		{
			if (vx > 0)
				aniId = ID_ANI_GREEN_KOOPAS_WALKING_RIGHT;
			else
				aniId = ID_ANI_GREEN_KOOPAS_WALKING_LEFT;
		}
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

	if (state == KOOPAS_STATE_DIE)
	{
		if (isRed)
			aniId = ID_ANI_RED_KOOPAS_SHELL_DIE_BY_COLLISION;
		else
			aniId = ID_ANI_GREEN_KOOPAS_SHELL_DIE_BY_COLLISION;
	}
	if (state == KOOPAS_STATE_REGEN)
	{
		if (isUpsideDown)
		{
			if (isRed)
				aniId = ID_ANI_RED_KOOPAS_SHELL_REGEN_USD;
			else
				aniId = ID_ANI_GREEN_KOOPAS_SHELL_REGEN_USD;
		}
		else
		{
			if (isRed)
				aniId = ID_ANI_RED_KOOPAS_SHELL_REGEN;
			else
				aniId = ID_ANI_GREEN_KOOPAS_SHELL_REGEN;
		}
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
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
		ay = KOOPAS_GRAVITY;
		//ay = 0;
		break;

		// being attacked by tail : move a bit and then upside down
	case KOOPAS_STATE_SHELL_UPSIDE_DOWN:
		vx = KOOPAS_TAIL_WHOOP_SPEED_X * nx;
		vy = -KOOPAS_TAIL_WHOOP_SPEED_Y;
		isUpsideDown = true;
		isOnGround = false;
		break;

	case KOOPAS_STATE_REGEN:
		vx = 0;
		vy = 0;
		//ay = KOOPAS_GRAVITY;

		realRegen_start = GetTickCount64(); // time to go to phase 3: Koopas reborn
		break;

	case KOOPAS_STATE_WALKING:
		vx = -0.05f;
		isUpsideDown = false;

		ay = KOOPAS_GRAVITY;
		break;
	
	case KOOPAS_STATE_SHELL_MOVING:
		vx = KOOPAS_SHELL_SPEED * nx;
		ay = KOOPAS_GRAVITY;
		break;

	case KOOPAS_STATE_WING:
		if (!isRed)
		{
			vx = -KOOPAS_FLYING_SPEED_VX;
		}
		else
		{
			vy = RED_KOOPAS_FLYING_SPEED_VY;
			ay = 0;
		}
		break;

	case KOOPAS_STATE_DIE:
		die_start = GetTickCount64();
		vy = -KOOPAS_DYING_SPEED;
		//ay = KOOPAS_GRAVITY_DYING;
		break;

	case KOOPAS_STATE_HELD:
		ay = 0;
		vx = 0;
		vy = 0;
		regen_start = GetTickCount64();
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
		if (dynamic_cast<CBrick*>(obj) || dynamic_cast<CLuckyBlock*>(obj) || dynamic_cast<CBlock*>(obj) || dynamic_cast<CPlatform*>(obj) || dynamic_cast<CInvisibleBlock*>(obj))
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