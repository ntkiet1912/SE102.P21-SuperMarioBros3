#include "Tail.h"
#include "Goomba.h"
#include "Mario.h"
#include "Game.h"
#include "LuckyBlock.h"
#include "CollisionEffect.h"
#include "PlayScene.h"
#include "FirePiranha.h"

void CTail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - TAIL_WIDTH / 2;
	t = y - TAIL_HEIGHT / 2;
	r = l + TAIL_WIDTH;
	b = t + TAIL_HEIGHT;
}

bool isAABBOverlapping(CGameObject* a, CGameObject* b)
{
	float l1, t1, r1, b1;
	float l2, t2, r2, b2;

	a->GetBoundingBox(l1, t1, r1, b1);
	b->GetBoundingBox(l2, t2, r2, b2);

	return !(r1 <= l2 || r2 <= l1 || b1 <= t2 || b2 <= t1);
}

void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (auto obj : *coObjects)
	{
		if (isAABBOverlapping(this, obj))
		{
			float nx = 0, ny = 0;

			float ox, oy;
			obj->GetPosition(ox, oy);
			nx = (ox > x) ? -1.0f : 1.0f;
			ny = 0;

			LPCOLLISIONEVENT e = new CCollisionEvent(0.0f, nx, ny, 0.0f, 0.0f, obj, this);
			if (dynamic_cast<CGoomba*>(obj))
				OnCollisionWithGoomba(e);
			else if (dynamic_cast<CKoopas*>(obj))
				OnCollisionWithKoopas(e);
			else if (dynamic_cast<CLuckyBlock*>(obj))
				OnCollisionWithLuckyBlock(e);
			else if (dynamic_cast<CFirePiranha*>(e->obj))
				OnCollisionWithFirePiranha(e);
		}
	}
}
void CTail::effectSpawn(LPGAMEOBJECT e)
{
	// spawn a flash effect 
	float ex, ey;
	e->GetPosition(ex, ey);
	CCollisionEffect* effect = new CCollisionEffect(ex, ey);
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	currentScene->AddObject(effect);
}
void CTail::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (this->x > goomba->getX())
		goomba->setNx(-1);
	else
		goomba->setNx(1);
	if (goomba->GetState() != GOOMBA_STATE_DIE || goomba->GetState() != GOOMBA_STATE_DIE_BY_COLLISION)
	{
		goomba->SetState(GOOMBA_STATE_DIE_BY_COLLISION);
	}
	effectSpawn(goomba);
}
void CTail::OnCollisionWithLuckyBlock(LPCOLLISIONEVENT e)
{
	CLuckyBlock* lb = dynamic_cast<CLuckyBlock*>(e->obj);
		lb->setIsHit(true);

}
void CTail::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	CKoopas* koopa = dynamic_cast<CKoopas*>(e->obj);
	if (this->x > koopa->getX())
		koopa->setNx(-1);
	else
		koopa->setNx(1);
	koopa->SetState(KOOPAS_STATE_SHELL_UPSIDE_DOWN);
	//koopa->SetState(KOOPAS_STATE_SHELL);
	effectSpawn(koopa);
}

void CTail::OnCollisionWithFirePiranha(LPCOLLISIONEVENT e)
{
	CFirePiranha* fp = dynamic_cast<CFirePiranha*>(e->obj);
	fp->Delete();
	effectSpawn(fp);
}


