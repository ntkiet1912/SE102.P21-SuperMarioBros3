#include "Goomba.h"
#include "Mario.h"
#include "PlayScene.h"
#include "UpgradeMarioLevel.h"
#include "DeadZone.h"
CGoomba::CGoomba(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GOOMBA_STATE_DIE || state == GOOMBA_STATE_DIE_BY_COLLISION)
	{
		left = top = right = bottom = 0;
	}
	else
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT / 2 + 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = y + GOOMBA_BBOX_HEIGHT / 2;
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CGoomba*>(e->obj)) return;
	else if (dynamic_cast<CDeadZone*>(e->obj)) {
		SetState(GOOMBA_STATE_DIE);
		return;
	}
	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	int aniId = ID_ANI_GOOMBA_WALKING;
	if (state == GOOMBA_STATE_DIE)
	{
		aniId = ID_ANI_GOOMBA_DIE;
	}
	else if(state == GOOMBA_STATE_DIE_BY_COLLISION)
	{
		aniId = ID_ANI_GOOMBA_DIE_BY_COLLISION;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		die_start = GetTickCount64();
		y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case GOOMBA_STATE_DIE_BY_COLLISION:
		die_start = GetTickCount64();
		//y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = GOOMBA_VX_DIE_SPEED;
		vy = -GOOMBA_DYING_SPEED;
		ay = GOOMBA_GRAVITY_DYING;
		break;
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		break;
	}
}

////////////////////////////RED GOOMBA/////////////////////////////////

CRedGoomba::CRedGoomba(float x, float y) : CGoomba(x, y)
{
	// init first phase 
	isWalking = true;
	walking_start = GetTickCount64();

	// init second phase 
	isBoucingUp = false;
	bouncing_start = 0;

	// init third phase 
	isFlyingUp = false;
	flying_start = 0;

	detect_start = GetTickCount64();
	state = GOOMBA_STATE_YES_WING;
}
void CRedGoomba::Render()
{
	int aniId;
	if (state == GOOMBA_STATE_DIE)
	{
		aniId = ID_ANI_RED_GOOMBA_DIE;
	}
	else if (state == GOOMBA_STATE_DIE_BY_COLLISION)
	{
		aniId = ID_ANI_RED_GOOMBA_DIE_BY_COLLISION;
	}
	else if (state == GOOMBA_STATE_YES_WING)
	{
		if (isWalking)
			aniId = ID_ANI_RED_GOOMBA_YESWING_WALKING;
		else if (isBoucingUp)
			aniId = ID_ANI_RED_GOOMBA_YESWING_START_FLYING;
		else
			aniId = ID_ANI_RED_GOOMBA_YESWING_FLYING;

	}
	else
		aniId = ID_ANI_RED_GOOMBA_NOWING_WALKING;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}
void CRedGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	// die
	if ((state == GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}
	if ((state == GOOMBA_STATE_DIE_BY_COLLISION) && (GetTickCount64() - die_start > 1500))
	{
		isDeleted = true;
		return;
	}

	/*
		with the wing, Goomba has even 3 phases:
			+ moving 2-3 steps
			+ bouncing 3 times
			+ flying
		it is a circulation process so when a phase end, another phase go up
		when last phase end, move to first phase
	*/
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (state == GOOMBA_STATE_YES_WING)
	{

		ay = GOOMBA_GRAVITY_FLYING;
		//first phase : walking 
		if (isWalking)
		{
			if (GetTickCount64() - walking_start > 600)
			{
				isWalking = false;
				isBoucingUp = true;
				bouncing_start = GetTickCount64();
				bounceCount = 3;
			}
			// a delay in "detect" where mario is -> avoid bug when mario stay in one position
			// & look like real game 
			if (GetTickCount64() - detect_start > 250)
			{
				detect_start = GetTickCount64();
				if (this->x < mario->getX())
					vx = GOOMBA_WALKING_SPEED;
				else
					vx = -GOOMBA_WALKING_SPEED;
			}
		}
		// second phase: bouncing 
		else if (isBoucingUp)
		{
			if (GetTickCount64() - bouncing_start > 350 && bounceCount >= 0)
			{
				vy = -GOOMBA_BOUNCE_SPEED;
				bouncing_start = GetTickCount64();
				bounceCount--;
			}
			else if (bounceCount < 0)
			{
				isBoucingUp = false;
				isFlyingUp = true;
				flying_start = GetTickCount64();
				vy = -GOOMBA_FLYING_SPEED;
			}
		}
		// third phase: flying 
		else if (isFlyingUp)
		{
			if (GetTickCount64() - flying_start > 800)
			{
				vy = 0;
				isFlyingUp = false;
				isWalking = true;
				walking_start = GetTickCount64();
			}
		}

	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CRedGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		die_start = GetTickCount64();
		y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case GOOMBA_STATE_DIE_BY_COLLISION:
		die_start = GetTickCount64();
		//y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = GOOMBA_VX_DIE_SPEED;
		vy = -GOOMBA_DYING_SPEED;
		ay = GOOMBA_GRAVITY_DYING;
		break;
	case GOOMBA_STATE_NO_WING:
		vx = -GOOMBA_WALKING_SPEED;
		break;
	case GOOMBA_STATE_YES_WING:
		vx = -GOOMBA_WALKING_SPEED;
		break;

	}
}
void CRedGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GOOMBA_STATE_DIE || state == GOOMBA_STATE_DIE_BY_COLLISION)
	{
		left = top = right = bottom = 0;
	}
	else if (state == GOOMBA_STATE_YES_WING)
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y  - GOOMBA_BBOX_WING_HEIGHT / 2 + 8;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = y + GOOMBA_BBOX_WING_HEIGHT / 2;
	}
	else
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
}