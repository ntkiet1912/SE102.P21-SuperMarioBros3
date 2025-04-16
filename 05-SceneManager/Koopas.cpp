#include "Koopas.h"
#include "Debug.h"
#include "Brick.h"
#include "Goomba.h"
#include "Portal.h"
#include "Platform.h"
#include "Coin.h"
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
	// shell state
	if (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_SHELLIDLE_MOVING_LEFT || state == KOOPAS_STATE_SHELLIDLE_MOVING_RIGHT)
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_SHELL_HEIGHT / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_SHELL_HEIGHT;
	}
	// when die, no bounding box for unexpected error
	else if (die_start)
	{
		left = 0;
		top =0;
		right =0;
		bottom =0;
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

	// collide with Block brick

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CPlatform*>(e->obj))
		OnCollisionWithPlatform(e);
	else if ((dynamic_cast<CKoopas*>(e->obj)))
		OnCollisionWithKoopas(e);

}
void CKoopas::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	CPlatform* platform = dynamic_cast<CPlatform*>(e->obj);
	if (isRed)
	{
		// platform rounding box
		float top, left, bottom, right;
		platform->GetBoundingBox(left, top, right, bottom);
		// position of Koopas
		float x, y;
		this->GetPosition(x, y);

		if (e->ny < 0) // on platform
		{
			// if reached the bound of platform
			if (x + KOOPAS_BBOX_WIDTH / 2 > right)
			{
				if (state == KOOPAS_STATE_WALKING_RIGHT)
					SetState(KOOPAS_STATE_WALKING_LEFT);
				else if (state == KOOPAS_STATE_SHELLIDLE_MOVING_RIGHT)
					SetState(KOOPAS_STATE_SHELLIDLE_MOVING_LEFT);
			}
			else if (x - KOOPAS_BBOX_WIDTH / 2 < left)
			{
				if (state == KOOPAS_STATE_WALKING_LEFT)
					SetState(KOOPAS_STATE_WALKING_RIGHT);
				else if (state == KOOPAS_STATE_SHELLIDLE_MOVING_LEFT)
					SetState(KOOPAS_STATE_SHELLIDLE_MOVING_RIGHT);
			}
		}
	}
}

void CKoopas::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	if (!isShellIdle) return;
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (state == KOOPAS_STATE_SHELLIDLE_MOVING_LEFT || state == KOOPAS_STATE_SHELLIDLE_MOVING_RIGHT)
	{
		if (e->nx != 0)
			goomba->SetState(GOOMBA_STATE_DIE);
	}
}
void CKoopas::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	if (!isShellIdle) return;
	else
	{
		CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);	
		if (e->nx != 0 || e->ny !=0)
		{
			if (state == KOOPAS_STATE_SHELLIDLE_MOVING_LEFT || state == KOOPAS_STATE_SHELLIDLE_MOVING_RIGHT)
			{
				koopas->SetState(KOOPAS_STATE_SHELL_DIE_BY_COLLISION_WITH_KOOPAS);
				if(e->nx > 0)
					koopas->vx = -KOOPAS_VX_DIE_SPEED;
				else 
					koopas->vx = KOOPAS_VX_DIE_SPEED;

			}
		}

	}
}


void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

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
	// die by collide with koopas shell moving
	if (((state == KOOPAS_STATE_WALK_DIE_BY_COLLISION_WITH_KOOPAS) ||
		(state == KOOPAS_STATE_SHELL_DIE_BY_COLLISION_WITH_KOOPAS)) &&
		(GetTickCount64() - die_start > 1500))
	{
		isDeleted = true;
		return;
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
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_SHELL:
		shellIdle_start = GetTickCount64();
		vx = 0;
		vy = 0;
		isShellIdle = true;

		//ay = 0;
		break;

	case KOOPAS_STATE_WALKING_LEFT:
		vx = -KOOPAS_WALKING_SPEED;
		isShellIdle = false;

		break;

	case KOOPAS_STATE_WALKING_RIGHT:
		vx = KOOPAS_WALKING_SPEED;
		isShellIdle = false;

		break;

	case KOOPAS_STATE_SHELLIDLE_MOVING_RIGHT:
		vx = KOOPAS_SHELL_SPEED;
		isShellIdle = true;

		//ay = KOOPAS_GRAVITY;
		break;

	case KOOPAS_STATE_SHELLIDLE_MOVING_LEFT:
		vx = -KOOPAS_SHELL_SPEED;
		isShellIdle = true;

		//ay = KOOPAS_GRAVITY;
		break;

	case KOOPAS_STATE_WING:
		// not done yet 
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

