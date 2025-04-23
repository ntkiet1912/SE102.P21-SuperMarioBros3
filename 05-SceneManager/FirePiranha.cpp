#include "FirePiranha.h"
#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"
#include "Sprites.h"
#include "Textures.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "FireBullet.h"

CFirePiranha::CFirePiranha(float x, float y) : CGameObject(x, y)
{
	startY = y;
	targetY = y - PIRANHA_MAX_RISE;
	direction = -1;
	facingVertical = -1;
	state = -1; 
	hasFired = false; // Chưa bắn
	stateTimer = GetTickCount64();
}

void CFirePiranha::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + FIRE_PIRANHA_WIDTH;
	bottom = top + FIRE_PIRANHA_HEIGHT;
}

bool CFirePiranha::IsPlayerTooClose()
{
	CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
	if (scene == nullptr) return false;

	LPGAMEOBJECT player = scene->GetPlayer();
	if (player == nullptr) return false;

	float playerX, playerY;
	player->GetPosition(playerX, playerY);
	return abs(playerX - x) < 30 && abs(playerY - y) < 50;
}

void CFirePiranha::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (IsPlayerTooClose()) return;

	switch (state)
	{
	case -1: 
		if (GetTickCount64() - stateTimer >= PIRANHA_HIDE_DELAY)
		{
			UpdateDirection(); 
			y -= PIRANHA_RISE_SPEED * dt;

			if (y <= targetY)
			{
				y = targetY;

				
				if (facingVertical == -1) 
					SetState(direction == 1 ? PIRANHA_STATE_FIRE_UP_LEFT : PIRANHA_STATE_FIRE_UP_RIGHT);
				else 
					SetState(direction == 1 ? PIRANHA_STATE_FIRE_DOWN_LEFT : PIRANHA_STATE_FIRE_DOWN_RIGHT);
			}
		}
		break;

	case PIRANHA_STATE_FIRE_UP_LEFT:
	case PIRANHA_STATE_FIRE_UP_RIGHT:
	case PIRANHA_STATE_FIRE_DOWN_LEFT:
	case PIRANHA_STATE_FIRE_DOWN_RIGHT:
		if (!hasFired) {
			Fire();
			hasFired = true; 
		}
		if (GetTickCount64() - stateTimer >= PIRANHA_RISE_DELAY)
		{
			y += PIRANHA_RISE_SPEED * dt;

			if (y >= startY)
			{
				y = startY;
				hasFired = false; 
				SetState(-1); 
			}

		}
		break;
	}
}

void CFirePiranha::UpdateDirection()
{
	CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
	if (scene == nullptr) return;

	LPGAMEOBJECT player = scene->GetPlayer();
	if (player == nullptr) return;

	float playerX, playerY;
	player->GetPosition(playerX, playerY);

	direction = (playerX > x ) ? -1 : 1;
	facingVertical = (playerY > y) ? 1 : -1;
}

void CFirePiranha::Fire()
{
	CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
	if (scene == nullptr) return;

	LPGAMEOBJECT player = scene->GetPlayer();
	if (player == nullptr) return;

	float playerX, playerY;
	player->GetPosition(playerX, playerY);

	float enemyX, enemyY;
	this->GetPosition(enemyX, enemyY);

	LPGAMEOBJECT fireBullet = new CFireBullet(enemyX, enemyY, playerX, playerY);

	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddObject(fireBullet);
	DebugOut(L"[Piranha] Fire!\n");
}

void CFirePiranha::SetState(int state)
{
	CGameObject::SetState(state);
	stateTimer = GetTickCount64();
}

void CFirePiranha::Render()
{
	int spriteId = -1;

	switch (state)
	{
	case PIRANHA_STATE_FIRE_UP_LEFT:
		spriteId = ID_SPRITE_PIRANHA_FIRE_UP_LEFT;
		break;

	case PIRANHA_STATE_FIRE_UP_RIGHT:
		spriteId = ID_SPRITE_PIRANHA_FIRE_UP_RIGHT;
		break;

	case PIRANHA_STATE_FIRE_DOWN_LEFT:
		spriteId = ID_SPRITE_PIRANHA_FIRE_DOWN_LEFT;
		break;

	case PIRANHA_STATE_FIRE_DOWN_RIGHT:
		spriteId = ID_SPRITE_PIRANHA_FIRE_DOWN_RIGHT;
		break;

	case -1:
		spriteId = ID_SPRITE_PIRANHA_FIRE_DOWN_LEFT;
		break;
	}

	if (spriteId != -1)
	{
		CSprites::GetInstance()->Get(spriteId)->Draw(x, y);
	}
	RenderBoundingBox();
}
