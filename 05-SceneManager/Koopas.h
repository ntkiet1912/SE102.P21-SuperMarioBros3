#pragma once
#include "GameObject.h"
#define KOOPAS_GRAVITY 0.001f
#define KOOPAS_WALKING_SPEED 0.05f
#define KOOPAS_SHELL_SPEED 0.22f
#define KOOPAS_JUMP_DEFLECT_SPEED 0.4f

#define KOOPAS_VX_DIE_SPEED 0.15f
#define KOOPAS_DYING_SPEED 0.29f

#define RED_KOOPAS_FLYING_SPEED_VY 0.05f 

#define KOOPAS_TAIL_WHOOP_SPEED_X 0.1f
#define KOOPAS_TAIL_WHOOP_SPEED_Y 0.25f

#define KOOPAS_FLYING_SPEED_VY 0.25f
#define KOOPAS_GRAVITY_FLYING 0.0007f
#define KOOPAS_FLYING_SPEED_VX 0.045f

#define KOOPAS_BBOX_WIDTH 16	
#define KOOPAS_BBOX_HEIGHT 26
#define KOOPAS_BBOX_SHELL_HEIGHT 15
#define KOOPAS_BBOX_REGEN_WIDTH 18

#define KOOPAS_SHELL_TIMEOUT 1000 

#pragma region state

#define KOOPAS_STATE_WALKING 1000
#define KOOPAS_STATE_SHELL 1001
#define KOOPAS_STATE_SHELL_UPSIDE_DOWN 1002
#define KOOPAS_STATE_SHELL_MOVING 1003
#define KOOPAS_STATE_HELD 1004

#define KOOPAS_STATE_REGEN 1005
#define KOOPAS_STATE_DIE 1006

#define KOOPAS_STATE_WING 1007
#pragma endregion



#define ID_ANI_RED_KOOPAS_WALKING_LEFT 20001
#define ID_ANI_RED_KOOPAS_WALKING_RIGHT 20002
#define ID_ANI_RED_KOOPAS_WING 20011
#define ID_ANI_RED_KOOPAS_SHELL 20021
#define ID_ANI_RED_KOOPAS_SHELL_MOVING 20022
#define ID_ANI_RED_KOOPAS_SHELL_REGEN 20031
#define ID_ANI_RED_KOOPAS_WALK_DIE_BY_COLLISION 20041
#define ID_ANI_RED_KOOPAS_SHELL_DIE_BY_COLLISION 20042
#define ID_ANI_RED_KOOPAS_SHELL_USD 20050
#define ID_ANI_RED_KOOPAS_SHELL_REGEN_USD 20052
#define ID_ANI_RED_KOOPAS_SHELL_MOVING_USD	20053

#define ID_ANI_GREEN_KOOPAS_WALKING_LEFT 21001
#define ID_ANI_GREEN_KOOPAS_WALKING_RIGHT 21002
#define ID_ANI_GREEN_KOOPAS_WING 21011
#define ID_ANI_GREEN_KOOPAS_SHELL 21021
#define ID_ANI_GREEN_KOOPAS_SHELL_MOVING 21022
#define ID_ANI_GREEN_KOOPAS_SHELL_REGEN 21031
#define ID_ANI_GREEN_KOOPAS_WALK_DIE_BY_COLLISION 21041
#define ID_ANI_GREEN_KOOPAS_SHELL_DIE_BY_COLLISION 21042
#define ID_ANI_GREEN_KOOPAS_SHELL_USD 21050
#define ID_ANI_GREEN_KOOPAS_SHELL_REGEN_USD 21052
#define ID_ANI_GREEN_KOOPAS_SHELL_MOVING_USD	21053

class CKoopas : public CGameObject
{
protected:
	ULONGLONG die_start;
	ULONGLONG regen_start;
	ULONGLONG realRegen_start;

	int isRed;
	int yesWing;

	bool isRegen;
	float ax;
	float ay;

	bool isOnGround;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; };
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	//void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopas(LPCOLLISIONEVENT e);
	void OnCollisionWithLuckyBlock(LPCOLLISIONEVENT e);
	void OnCollisionWithGoldenBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithButtonBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithPiranha(LPCOLLISIONEVENT e);
	bool IsTherePlatformAhead(vector<LPGAMEOBJECT>* coObjects);

public:
	bool isUpsideDown;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	CKoopas(float x, float y, int isRed, int yesWing);
	virtual void SetState(int state);
	virtual void GetStomped();
};
