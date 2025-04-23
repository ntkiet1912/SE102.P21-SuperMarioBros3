#pragma once
#include "GameObject.h"
#define KOOPAS_GRAVITY 0.002f
#define KOOPAS_WALKING_SPEED 0.05f
#define KOOPAS_SHELL_SPEED 0.25f
#define KOOPAS_VX_DIE_SPEED 0.1f
#define KOOPAS_JUMP_DEFLECT_SPEED 0.4f
#define KOOPAS_DYING_SPEED 0.2f
#define KOOPAS_GRAVITY_DYING 0.0003f
#define KOOPAS_FLYING_SPEED 0.4f
#define KOOPAS_GRAVITY_FLYING 0.002f


#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 26
#define KOOPAS_BBOX_SHELL_HEIGHT 16
#define KOOPAS_BBOX_REGEN_WIDTH 18

#define KOOPAS_SHELL_TIMEOUT 1000 

#define KOOPAS_STATE_WALKING_LEFT 1000
#define KOOPAS_STATE_WALKING_RIGHT 1001
#define KOOPAS_STATE_SHELL 1002
#define KOOPAS_STATE_SHELLIDLE_MOVING_LEFT 1003
#define KOOPAS_STATE_SHELLIDLE_MOVING_RIGHT 1004
#define KOOPAS_STATE_WING 1005
#define KOOPAS_STATE_WALK_DIE_BY_COLLISION_WITH_KOOPAS 1006
#define KOOPAS_STATE_SHELL_DIE_BY_COLLISION_WITH_KOOPAS 1007
#define KOOPAS_STATE_REGEN 1008

//#define KOOPAS_STATE_IS_HELD 1009


#define ID_ANI_RED_KOOPAS_WALKING_LEFT 20001
#define ID_ANI_RED_KOOPAS_WALKING_RIGHT 20002
#define ID_ANI_RED_KOOPAS_WING 20011
#define ID_ANI_RED_KOOPAS_SHELL 20021
#define ID_ANI_RED_KOOPAS_SHELL_MOVING 20022
#define ID_ANI_RED_KOOPAS_SHELL_REGEN 20031
#define ID_ANI_RED_KOOPAS_WALK_DIE_BY_COLLISION 20041
#define ID_ANI_RED_KOOPAS_SHELL_DIE_BY_COLLISION 20042

#define ID_ANI_GREEN_KOOPAS_WALKING_LEFT 21001
#define ID_ANI_GREEN_KOOPAS_WALKING_RIGHT 21002
#define ID_ANI_GREEN_KOOPAS_WING 21011
#define ID_ANI_GREEN_KOOPAS_SHELL 21021
#define ID_ANI_GREEN_KOOPAS_SHELL_MOVING 21022
#define ID_ANI_GREEN_KOOPAS_SHELL_REGEN 21031
#define ID_ANI_GREEN_KOOPAS_WALK_DIE_BY_COLLISION 21041
#define ID_ANI_GREEN_KOOPAS_SHELL_DIE_BY_COLLISION 21042


class CKoopas : public CGameObject
{
protected:
	bool isShellIdle;

	ULONGLONG die_start;
	ULONGLONG flying_start;
	ULONGLONG regen_start;
	ULONGLONG realRegen_start;

	int isRed;
	int yesWing;

	bool isRegen;
	bool isFlyingUp;
	float ax;
	float ay;

	bool isHeld = false;
	bool isReleased = false;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; };
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopas(LPCOLLISIONEVENT e);


public:
	CKoopas(float x, float y, int isRed, int yesWing);
	virtual void SetState(int state);
	void setIsHeld(bool isHeld) { this->isHeld = isHeld; }
	void setIsReleased(bool isHeld) { this->isReleased = isReleased; }
};
