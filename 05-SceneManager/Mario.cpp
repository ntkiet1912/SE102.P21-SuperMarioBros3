#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"
#include "PlayHUD.h"
#include "DataManager.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"
#include "FirePiranha.h"
#include "FireBullet.h"
#include "Koopas.h"

#include "Collision.h"
#include "LuckyBlock.h"
#include "UpgradeMarioLevel.h"
#include "GoalRoulette.h"
#include "PlayScene.h"
#include "FlyingGround.h"

float speedMeter = 0.0f;
const float SPEED_METER_MAX = 1.0f; 
const float SPEED_METER_FILL_RATE = 0.001f;
const float SPEED_METER_DECAY_RATE = 0.002f;
void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	camSpeed = 0.01f * dt;
	vx += ax * dt;
	vy += ay * dt;
	if (state == MARIO_ENDING_SCENE)
	{
		//if (isOnPlatform)
		//	vx = MARIO_WALKING_SPEED;
		//else
		//	vx = 0;
	}

	if (abs(vx) > abs(maxVx))
	{
		vx = maxVx;
	}

	// when mario reach max walking speed: gradually run
	if (isOnPlatform)
	{
		if (abs(vx) > MARIO_WALKING_SPEED)
		{
			speedMeter += SPEED_METER_FILL_RATE * dt;
			if (speedMeter > SPEED_METER_MAX) speedMeter = SPEED_METER_MAX;
		}
		else
		{
			speedMeter -= SPEED_METER_DECAY_RATE * dt;
			if (speedMeter < 0) speedMeter = 0;
		}
		isRunning = (speedMeter >= SPEED_METER_MAX);
	}

	// when reach max speed_meter: mario is running

	if (level == 3 && !isOnPlatform && canFly)
	{
		bool isJumpKeyDown = CGame::GetInstance()->IsKeyDown(DIK_S);
		bool isJumpKeyPressed = CGame::GetInstance()->IsKeyPress(DIK_S);

		if (isRunning && isJumpKeyPressed)
		{
			vy = -MARIO_FLY_UP_SPEED;
			isWagFlyingUp = true;
			isFlying = true;
			flying_start = GetTickCount64();
		}
		else if (!isRunning && isJumpKeyPressed)
		{
			// Apply reduced gravity for tail flap descent
			vy += MARIO_TAIL_FLAP_GRAVITY_REDUCE * dt;
			if (vy > MARIO_MIN_FALL_SPEED)
				vy = MARIO_MIN_FALL_SPEED;
			isWagDown = true;
			isFlying = true;
			flying_start = GetTickCount64();
		}
	}
	if (isFlying && GetTickCount64() - flying_start > 300)
	{
		isFlying = false;
		isWagFlyingUp = false;
		isWagDown = false;
	}
	if (isLevelUp && GetTickCount64() - transformation_start > TRANSFORMATION_DURATION) {
		isLevelUp = false;
		isActive = true;
		transformation_start = -1;
	}

	if (isLevelDown && GetTickCount64() - transformation_start > TRANSFORMATION_DURATION) {
		isLevelDown = false;
		if (level == 3)
			SetLevel(2);
		else
			SetLevel(1);
		isActive = true;
		transformation_start = -1;
		StartUntouchable();
	}

	if (GetTickCount64() - tailAttack_start > TAIL_ATTACK_DURATION)
	{
		isTailAttacking = false;
		if (tail != nullptr)
		{
			tail->Delete();
			tail = nullptr;
		}
	}
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
	tailUpdate(dt, coObjects);
	holdingKoopas();
	liftUpdate(dt, coObjects);
	//DebugOut(L"[CMario] isOnPlatform: %d\n", isOnPlatform);
	//DebugOut(L"[CMario] state: %d\n", state);

} 
void CMario::holdingKoopas()
{
	if (isHolding)
	{
		if (canHold)
			PositionHeldKoopas(heldKoopas);
		else
		{
			isHolding = false;
			kickShell(heldKoopas);
			heldKoopas->SetState(KOOPAS_STATE_SHELL_MOVING);
			heldKoopas = nullptr;
		}
	}

}
void CMario::tailUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (tail != nullptr)
	{
		if (nx > 0)
			tail->SetPosition(x + 6, y + 3);
		else
			tail->SetPosition(x - 6, y + 3);

		tail->Update(dt, coObjects);
	}
}
void CMario::PositionHeldKoopas(LPGAMEOBJECT koopas)
{
	// if mario state != small, then the offset is different a bit.
	float offsetX, offsetY;
	if (level == MARIO_LEVEL_BIG)
	{
		offsetX = 12.0f;
		offsetY = -4.0f;
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
void CMario::liftUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (currentFlyingGround != nullptr)
	{
		// to check that mario is on the flying ground
		// use Raycasting to check (epsilon = 1.0f) like the gap between mario and the platform
		float mLeft, mTop, mRight, mBottom;
		float pLeft, pTop, pRight, pBottom;
		GetBoundingBox(mLeft, mTop, mRight, mBottom);
		currentFlyingGround->GetBoundingBox(pLeft, pTop, pRight, pBottom);

		const float epsilon = 1.0f;
		if (abs(mBottom - pTop) <= epsilon && mRight > pLeft && mLeft < pRight)
		{
			// sync mario's position & vy with the fg
			// to make it move smoothly
			y = currentFlyingGround->getY() - 8 - getHeight() / 2;
			vy = currentFlyingGround->getVy();
			isOnPlatform = true;
		}
		else // when mario jump out or move out of the fg
		{
			currentFlyingGround = nullptr;
		}
	}
}
void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
}
#pragma region COLLISION

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{

	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0)
		{
			isOnPlatform = true;
			isWagDown = false;
			isWagFlyingUp = false;
		}

	}
	else
		if (e->nx != 0 && e->obj->IsBlocking())
		{
			vx = 0;
		}
	//DebugOut(L"vx = %f\n", vx);
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
	else if (dynamic_cast<CGoalRouletteIcon*>(e->obj))
		OnCollisionWithGoalRouletteIcon(e);
	else if (dynamic_cast<CBrick*>(e->obj))
		OnCollisionWithBrickWall(e);
	else if (dynamic_cast<CFlyingGround*>(e->obj))
		OnCollisionWithFlyingGround(e);
}
void CMario::OnCollisionWithFlyingGround(LPCOLLISIONEVENT e)
{
	CFlyingGround* flyingGround = dynamic_cast<CFlyingGround*>(e->obj);
	if (e->ny < 0) // Mario lands on top of the platform
	{
		isOnPlatform = true;
		currentFlyingGround = flyingGround;

		// Notify the platform that it has been stepped on
		// start descending
		flyingGround->setIsStepped(true);
	}
}
void CMario::OnCollisionWithBrickWall(LPCOLLISIONEVENT e)
{
	if (e->nx > 0)
	{
		float x, y;
		GetPosition(x, y);

		// Đẩy Mario ra khỏi tường một chút
		x += e->nx * (abs(e->dx) + 0.4f); // hoặc BLOCK_PUSH_FACTOR

		SetPosition(x, y);
		ax = 0;
		vx = -camSpeed; // Mario đi cùng tốc độ camera, hoặc để 0
	}
}
void CMario::OnCollisionWithFirePiranha(LPCOLLISIONEVENT e)
{
	getDmg();
}

void CMario::OnCollisionWithFireBullet(LPCOLLISIONEVENT e)
{
	getDmg();
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
			CDataManager::GetInstance()->AddScore(1000);
			CPlayHUD::GetInstance()->SetScore(CDataManager::GetInstance()->GetScore());
		}
		vy = -MARIO_JUMP_DEFLECT_SPEED;
	}
	else // hit by Goomba
	{
		getDmg();
	}
}

void checkNx(LPGAMEOBJECT x1, LPGAMEOBJECT x2)
{
	if (x1->getX() < x2->getX())
		x2->setNx(1);
	else
		x2->setNx(-1);
}
void CMario::kickShell(CKoopas*& koopas)
{
	if (this->nx > 0)
		koopas->setNx(1);
	else if (this->nx < 0)
		koopas->setNx(-1);
	koopas->SetState(KOOPAS_STATE_SHELL_MOVING);
	kick_flag = true;
	SetState(MARIO_STATE_KICK);
}
void CMario::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
	if (!koopas) return;

	float koopasX, koopasY;
	koopas->GetPosition(koopasX, koopasY);

	int stateKoopas = koopas->GetState();
	if (e->ny > 0 )
	{
		if (stateKoopas == KOOPAS_STATE_SHELL || stateKoopas == KOOPAS_STATE_SHELL_UPSIDE_DOWN) return;
		DebugOut(L"Getdmg from ny\n");

		getDmg();
		return;
	}

	// Jump on head
	if (e->ny < 0)
	{
		vy = -MARIO_JUMP_DEFLECT_SPEED;

		if (koopas->GetState() == KOOPAS_STATE_SHELL || koopas->GetState() == KOOPAS_STATE_REGEN)
		{
			checkNx(this, koopas);
			koopas->SetState(KOOPAS_STATE_SHELL_MOVING);
			CDataManager::GetInstance()->AddScore(1000);

		}
		else if (koopas->GetState() == KOOPAS_STATE_WING)
		{
			checkNx(this, koopas);
			koopas->SetState(KOOPAS_STATE_WALKING);
			CDataManager::GetInstance()->AddScore(1000);

		}
		else
		{
			koopas->SetState(KOOPAS_STATE_SHELL);
			CDataManager::GetInstance()->AddScore(1000);
		}
		return;
	}

	// Side collision
	if (e->nx != 0)
	{
		if (stateKoopas == KOOPAS_STATE_SHELL || stateKoopas == KOOPAS_STATE_HELD || stateKoopas == KOOPAS_STATE_REGEN)
		{
			if (!isHolding && canHold)
			{
				isHolding = true;
				heldKoopas = koopas;
				koopas->SetState(KOOPAS_STATE_HELD);
			}
			else if (!canHold)
			{
				kickShell(koopas);
			}
		}
		else
		{
			if(stateKoopas == KOOPAS_STATE_SHELL_UPSIDE_DOWN) return;
			DebugOut(L"Getdmg from nx\n");
			getDmg();
		}
	}
}


void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	CDataManager::GetInstance()->AddCoin(1);
	CDataManager::GetInstance()->AddScore(100);
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
	CDataManager::GetInstance()->AddScore(1000);

	if (dynamic_cast<CMushroom1UP*>(e->obj))
	{
		DebugOut(L"Life++\n");
	}
	else if (level == 1 && dynamic_cast<CMushroomUpgradingMarioLevel*>(e->obj))
	{
		levelUp();
	}
	else if (level >= 2 && dynamic_cast<CLeaf*>(e->obj))
	{
		if (level == 3)	CDataManager::GetInstance()->AddScore(1000);
		else levelUp();
	}
	e->obj->Delete();
}

void CMario::OnCollisionWithGoalRouletteIcon(LPCOLLISIONEVENT e)
{
	CGoalRouletteIcon* icon = dynamic_cast<CGoalRouletteIcon*>(e->obj);
	DebugOut(L"set\n");
	icon->setIsHitByMario(true);
	// mario is uncontrolable during this time
	SetState(MARIO_ENDING_SCENE);

}
#pragma endregion

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;

	if (state == MARIO_ENDING_SCENE)
	{
		aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
		return aniId;
	}
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
	else if (isHolding)
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
	if (state == MARIO_ENDING_SCENE)
	{
		aniId = ID_ANI_MARIO_WALKING_RIGHT;
		return aniId;
	}
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
	else if (isHolding)
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
				else if (isRunning)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else 
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (isRunning)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else 
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
	if (isWagDown)
	{
		if (nx > 0)
			aniId = ID_ANI_MARIO_WITH_TAIL_WAG_DOWN_RIGHT;
		else
			aniId = ID_ANI_MARIO_WITH_TAIL_WAG_DOWN_LEFT;
		return aniId;
	}
	if (isWagFlyingUp)
	{
		if (nx > 0)
			aniId = ID_ANI_MARIO_WITH_TAIL_WAG_UP_RIGHT;
		else
			aniId = ID_ANI_MARIO_WITH_TAIL_WAG_UP_LEFT;
		return aniId;
	}
	if (isTailAttacking)
	{
		if (nx > 0)
		{
			aniId = ID_ANI_MARIO_WITH_TAIL_ATTACK_LEFT;
		}
		else
			aniId = ID_ANI_MARIO_WITH_TAIL_ATTACK_RIGHT;
		return aniId;
	}
	if (state == MARIO_ENDING_SCENE)
	{
		aniId = ID_ANI_MARIO_WITH_TAIL_WALKING_RIGHT;
		return aniId;
	}
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
	else if (isHolding)
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
	if (untouchable)
	{
		// skip sprite 
		if ((GetTickCount64() / 50) % 2 == 0)
			return;
	}

	if (isLevelDown)
	{
		if (level == 3)
		{
			aniId = ID_ANI_MARIO_PUFF;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SHRINK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SHRINK_LEFT;
		}
	}
	else if (isLevelUp)
	{
		if (level == 3)
		{
			aniId = ID_ANI_MARIO_PUFF;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_EXPAND_RIGHT;
			else
				aniId = ID_ANI_MARIO_EXPAND_LEFT;
		}
	}
	else if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_WITH_TAIL)
		aniId = GetAniIdWithTail();
	animations->Get(aniId)->Render(x, y);

	RenderBoundingBox();

	//DebugOutTitle(L"Coins: %d", coin);
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
		//case MARIO_STATE_JUMP:
		//	if (isSitting) break;
		//	if (isOnPlatform)
		//	{
		//		if (abs(this->vx) == MARIO_RUNNING_SPEED)
		//			vy = -MARIO_JUMP_RUN_SPEED_Y;
		//		else
		//			vy = -MARIO_JUMP_SPEED_Y;
		//	}
		//	break;

		//case MARIO_STATE_RELEASE_JUMP:
		//	if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 16;
		//	break;

	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			jump_hold_start = GetTickCount64();
			if (abs(vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0)
		{
			ULONGLONG now = GetTickCount64();
			ULONGLONG hold_time = now - jump_hold_start;

			float factor = (float)hold_time / 200.0f; // Giới hạn 200ms là nhảy tối đa
			if (factor > 1.0f) factor = 1.0f;
			else if (factor < 0.5f) factor = 0.5f;
			// Làm Mario rơi sớm hơn nếu nhả phím sớm
			vy += (1.0f - factor) * MARIO_JUMP_SPEED_Y;
		}
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
	case MARIO_ENDING_SCENE:
		break;
	case MARIO_STATE_TAIL_ATTACK:
		if (!isTailAttacking)
		{
			isTailAttacking = true;

			int tailNx = nx > 0 ? 1 : 0;
			tail = new CTail(this->x, y + 3, tailNx);
			tail->isActive = true;
			CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
			scene->AddObject(tail);
			tailAttack_start = GetTickCount64();
			//vector<LPGAMEOBJECT>& objects = scene->GetObjects();
			//objects.push_back(tail);
		}
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

void CMario::levelDown()
{
	isLevelDown = true;
	transformation_start = GetTickCount64();
	isActive = false;
	vx = 0;
	ax = 0;
}
void CMario::levelUp()
{
	isLevelUp = true;
	transformation_start = GetTickCount64();
	isActive = false;
	vx = 0;
	ax = 0;
	if (level == 1)
		SetLevel(2);
	else if (level == 2)
		SetLevel(3);
}
void CMario::getDmg()
{
	if (untouchable == 0)
	{
		if (level == (MARIO_LEVEL_WITH_TAIL))
		{
			levelDown();
		}
		else if (level == MARIO_LEVEL_BIG)
		{
			levelDown();
		}
		else
			SetState(MARIO_STATE_DIE);
	}
}

int CMario::getHeight()
{
	if (level == 1)
	{
		return MARIO_SMALL_BBOX_HEIGHT;
	}
	return MARIO_BIG_BBOX_HEIGHT;
}