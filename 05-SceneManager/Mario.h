#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"
#include "Koopas.h"
#include "debug.h"
#include "Tail.h"
#include "FlyingGround.h"

#pragma region Constaint

#define MARIO_WALKING_SPEED		0.094f
#define MARIO_RUNNING_SPEED		0.16f

#define MARIO_ACCEL_WALK_X	0.00025f
#define MARIO_ACCEL_RUN_X	0.00027f

#define MARIO_JUMP_SPEED_Y		0.44f
#define MARIO_JUMP_RUN_SPEED_Y	0.51f

#define MARIO_GRAVITY			0.0012f

#define MARIO_JUMP_DEFLECT_SPEED  0.25f

#define MARIO_FLY_UP_SPEED 0.25f
#define MARIO_TAIL_FLAP_GRAVITY_REDUCE 0.001f
#define MARIO_MIN_FALL_SPEED 0.01f
#pragma endregion

#pragma region State

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_STATE_KICK	701
#define MARIO_STATE_TAIL_ATTACK 711
#define MARIO_ENDING_SCENE	999

#pragma endregion

#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601 

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

#define ID_ANI_MARIO_DIE 999


#define ID_ANI_MARIO_KICK_LEFT 1010
#define ID_ANI_MARIO_KICK_RIGHT 1011

#define ID_ANI_MARIO_STANDING_HOLDSHELL_LEFT 1020
#define ID_ANI_MARIO_STANDING_HOLDSHELL_RIGHT 1021
#define ID_ANI_MARIO_RUNNING_HOLDSHELL_LEFT 1025
#define ID_ANI_MARIO_RUNNING_HOLDSHELL_RIGHT 1026
// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601


#define ID_ANI_MARIO_SMALL_KICK_LEFT 1610
#define ID_ANI_MARIO_SMALL_KICK_RIGHT 1611

#define ID_ANI_MARIO_SMALL_STANDING_HOLDSHELL_LEFT 1620
#define ID_ANI_MARIO_SMALL_STANDING_HOLDSHELL_RIGHT 1621
#define ID_ANI_MARIO_SMALL_RUNNING_HOLDSHELL_LEFT 1625
#define ID_ANI_MARIO_SMALL_RUNNING_HOLDSHELL_RIGHT 1626

// MARIO WITH TAIL
#define ID_ANI_MARIO_WITH_TAIL_IDLE_RIGHT -400
#define ID_ANI_MARIO_WITH_TAIL_IDLE_LEFT -401

#define ID_ANI_MARIO_WITH_TAIL_WALKING_RIGHT -500
#define ID_ANI_MARIO_WITH_TAIL_WALKING_LEFT -501

#define ID_ANI_MARIO_WITH_TAIL_RUNNING_RIGHT -600
#define ID_ANI_MARIO_WITH_TAIL_RUNNING_LEFT -601 

#define ID_ANI_MARIO_WITH_TAIL_JUMP_WALK_RIGHT -700
#define ID_ANI_MARIO_WITH_TAIL_JUMP_WALK_LEFT -701
#define ID_ANI_MARIO_WITH_TAIL_JUMP_WALK_RIGHT_RELEASE -710
#define ID_ANI_MARIO_WITH_TAIL_JUMP_WALK_LEFT_RELEASE -711

#define ID_ANI_MARIO_WITH_TAIL_JUMP_RUN_RIGHT -800
#define ID_ANI_MARIO_WITH_TAIL_JUMP_RUN_LEFT -801
#define ID_ANI_MARIO_WITH_TAIL_JUMP_RUN_RIGHT_RELEASE -810
#define ID_ANI_MARIO_WITH_TAIL_JUMP_RUN_LEFT_RELEASE -811
#define ID_ANI_MARIO_WITH_TAIL_SIT_RIGHT -900
#define ID_ANI_MARIO_WITH_TAIL_SIT_LEFT -901

#define ID_ANI_MARIO_WITH_TAIL_BRACE_RIGHT -1000
#define ID_ANI_MARIO_WITH_TAIL_BRACE_LEFT -1001

#define ID_ANI_MARIO_WITH_TAIL_DIE -999

#define ID_ANI_MARIO_WITH_TAIL_KICK_LEFT -1010
#define ID_ANI_MARIO_WITH_TAIL_KICK_RIGHT -1011

#define ID_ANI_MARIO_WITH_TAIL_STANDING_HOLDSHELL_LEFT -1020
#define ID_ANI_MARIO_WITH_TAIL_STANDING_HOLDSHELL_RIGHT -1021
#define ID_ANI_MARIO_WITH_TAIL_RUNNING_HOLDSHELL_LEFT -1025
#define ID_ANI_MARIO_WITH_TAIL_RUNNING_HOLDSHELL_RIGHT -1026
#define ID_ANI_MARIO_WITH_TAIL_JUMP_HOLDSHELL_LEFT -1027
#define ID_ANI_MARIO_WITH_TAIL_JUMP_HOLDSHELL_RIGHT -1028
#define ID_ANI_MARIO_WITH_TAIL_ATTACK_LEFT -1029
#define ID_ANI_MARIO_WITH_TAIL_ATTACK_RIGHT -1030
#define ID_ANI_MARIO_WITH_TAIL_WAG_UP_LEFT -1031
#define ID_ANI_MARIO_WITH_TAIL_WAG_UP_RIGHT -1032
#define ID_ANI_MARIO_WITH_TAIL_WAG_DOWN_LEFT -1033
#define ID_ANI_MARIO_WITH_TAIL_WAG_DOWN_RIGHT -1034
#
// shrink 
#define ID_ANI_MARIO_SHRINK_LEFT	100
#define ID_ANI_MARIO_SHRINK_RIGHT	101

// expand 
#define ID_ANI_MARIO_EXPAND_LEFT 102
#define ID_ANI_MARIO_EXPAND_RIGHT 103
#define ID_ANI_MARIO_PUFF 104

#pragma endregion

#define GROUND_Y 160.0f




#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define	MARIO_LEVEL_WITH_TAIL	3

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16
#define MARIO_BIG_KICKING_BBOX_WIDTH  19

#define MARIO_BIG_HOLDSHELL_BBOX_WIDTH  16
#define MARIO_BIG_HOLDSHELL_BBOX_HEIGHT  24


#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12

#define MARIO_RACOON_BBOX_WIDTH  21
#define MARIO_RACOON_BBOX_HEIGHT 26

#define MARIO_UNTOUCHABLE_TIME 1500
#define TRANSFORMATION_DURATION 800
#define TRANSFORMATION_RACOON_DURATION 450
#define TAIL_TRANSFORMATION_DURATION 750

#define TAIL_ATTACK_DURATION 300

class CMario : public CGameObject
{
	BOOLEAN isSitting;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	int level;
	int untouchable;
	ULONGLONG untouchable_start;
	BOOLEAN isOnPlatform;
	int coin;

	ULONGLONG kick_start;
	bool kick_flag;

	bool canHold;
	bool isHolding;
	CKoopas* heldKoopas;

	bool canSit;
	bool isLevelUp;
	bool isLevelDown;
	bool isActive;
	ULONGLONG transformation_start;
	ULONGLONG jump_hold_start;

	CTail* tail;
	ULONGLONG tailAttack_start;
	ULONGLONG tail_spawn_time;
	bool isTailAttacking;

	CFlyingGround* currentFlyingGround;
	
	bool isFlying;
	ULONGLONG flying_start;
	bool isRunning;
	bool isWagFlyingUp;
	bool isWagDown;
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithFirePiranha(LPCOLLISIONEVENT e);
	void OnCollisionWithFireBullet(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopas(LPCOLLISIONEVENT e);
	void kickShell(CKoopas*& koopas);

	void OnCollisionWithLuckyBlock(LPCOLLISIONEVENT e);
	void OnCollisionWithGoalRouletteIcon(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdWithTail();

	void levelUp();
	void levelDown();
	void tailUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void holdingKoopas();
	void liftUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
public:
	bool canFly;
	float camSpeed;
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY;

		level = 3;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;

		kick_flag = false;
		kick_start = -1;

		canHold = false;
		isHolding = false;
		heldKoopas = nullptr;
		canSit = true;
		jump_hold_start = -1;

		isLevelUp = false;
		isLevelDown = false;
		transformation_start = -1;
		isActive = true;

		tail = nullptr;
		tailAttack_start = -1;
		tail_spawn_time = -1;
		isTailAttacking = false;

		currentFlyingGround = nullptr;
		isFlying = false;
		isRunning = false;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable(){return (state != MARIO_STATE_DIE);}

	int IsBlocking() { return 0; }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);


	void OnCollisionWithUpgradingItem(LPCOLLISIONEVENT e);
	void OnCollisionWithBrickWall(LPCOLLISIONEVENT e);
	void OnCollisionWithFlyingGround(LPCOLLISIONEVENT e);
	void getDmg();

	void setCanHold(bool canHold) { this->canHold = canHold; }
	void setIsHolding(bool isHolding) { this->isHolding = isHolding; }
	float getVx() { return vx; }
	int getLevel() { return level; }
	void setLevel(int lv) { this->level = lv; }
	bool getIsHolding() { return isHolding; }
	bool getCanHold() { return canHold; }
	void PositionHeldKoopas(LPGAMEOBJECT);
	void setCanSit(int cs) { this->canSit = cs; }
	bool getCanSit() { return canSit; }
	bool getIsUntouchable() { return untouchable; }
	bool getIsSitting() { return isSitting; }
	bool getIsActive() { return isActive; }
	void setVx(float vx) { this->vx = vx; }
	void setVy(float vy) { this->vy = vy; }
	int getHeight();

	void SetCurrentFlyingGround(CFlyingGround* fg) { currentFlyingGround = fg; }
	CFlyingGround* GetCurrentFlyingGround() { return currentFlyingGround; }

};