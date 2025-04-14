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
	die_start = -1;
	this->isRed = isRed;
	this->yesWing = yesWing;
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
	//if (!e->obj->IsBlocking()) return;


	if (e->ny != 0)
	{
		vy = 0;
	}
	if (dynamic_cast<CCoin*>(e->obj)) return;

	else if (e->nx != 0)
	{
		if (state == KOOPAS_STATE_WALKING_LEFT)
			SetState(KOOPAS_STATE_WALKING_RIGHT);
		else if (state == KOOPAS_STATE_WALKING_RIGHT)
			SetState(KOOPAS_STATE_WALKING_LEFT);
	}
	//else if (e->nx != 0)
	//{
	//	if (state == KOOPAS_STATE_SHELLIDLE_MOVING_LEFT || state == KOOPAS_STATE_SHELLIDLE_MOVING_RIGHT)
	//	{
	//		CGameObject* obj = dynamic_cast<CGameObject*>(e->obj);
	//		if (obj)
	//		{
	//			if (dynamic_cast<CBrick*>(obj)) return;

	//			obj->Delete();
	//			return;
	//		}
	//		vx = -vx;
	//	}
	//	if (state == KOOPAS_STATE_WALKING_LEFT)
	//		SetState(KOOPAS_STATE_WALKING_RIGHT);
	//	else if (state == KOOPAS_STATE_WALKING_RIGHT)
	//		SetState(KOOPAS_STATE_WALKING_LEFT);
	//}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CPlatform*>(e->obj))
		OnCollisionWithPlatform(e);

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
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (state == KOOPAS_STATE_SHELLIDLE_MOVING_LEFT || state == KOOPAS_STATE_SHELLIDLE_MOVING_RIGHT)
	{
		if (e->nx != 0)
			goomba->SetState(GOOMBA_STATE_DIE);
	}
}


void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == KOOPAS_STATE_SHELL) && (GetTickCount64() - shellIdle_start > KOOPAS_SHELL_TIMEOUT))
	{
		SetState(KOOPAS_STATE_SHELL);
		//isRegen = true;
	}
	//else if (state == KOOPAS_STATE_WALKING_LEFT)
	//{
	//	SetState(KOOPAS_STATE_WALKING_LEFT);
	//}
	//else if (state == KOOPAS_STATE_WALKING_RIGHT)
	//{
	//	SetState(KOOPAS_STATE_WALKING_RIGHT);
	//}
	//else if (state == KOOPAS_STATE_WING)
	//{
	//	SetState(KOOPAS_STATE_WING);
	//}
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
		vx = -KOOPAS_WALKING_SPEED;
		isShellIdle = false;
		//vy = -KOOPAS_JUMP_DEFLECT_SPEED;
		break;
	}
}

