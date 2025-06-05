#pragma once
#include "GameObject.h"
#include "PlayScene.h"
#include <vector>
using namespace std;
#define BROKEN_BRICK_EFFECT_TIMEOUT 1000
#define ID_ANI_PARTICLE_BRICK 90000
#define PARTICLE_GRAVITY 0.002f
#define PARTICLE_SPEED_X 0.1f
#define PARTICLE_SPEED_Y -0.4f
class CParticle : public CGameObject {
public:
    CParticle(float x, float y, float vx, float vy) :CGameObject(x, y) {
        this->vx = vx;
        this->vy = vy;
    }
    void Render() {
        CAnimations::GetInstance()->Get(ID_ANI_PARTICLE_BRICK)->Render(x, y);
    }
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
        vy += PARTICLE_GRAVITY * dt;
        x += vx * dt;
        y += vy * dt;
    }
    void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
    }
};
class CBrokenBrickEffect :
    public CGameObject
{
    ULONGLONG startTime = 0;
    vector<CParticle*> particles;
public:
    CBrokenBrickEffect(float x, float y) :CGameObject(x, y) {
        startTime = GetTickCount64();
        particles.push_back(new CParticle(x, y, PARTICLE_SPEED_X, PARTICLE_SPEED_Y));
        particles.push_back(new CParticle(x, y, -PARTICLE_SPEED_X, PARTICLE_SPEED_Y));
        particles.push_back(new CParticle(x, y, PARTICLE_SPEED_X, PARTICLE_SPEED_Y / 2));
        particles.push_back(new CParticle(x, y, -PARTICLE_SPEED_X, PARTICLE_SPEED_Y / 2));
    }
    ~CBrokenBrickEffect() {
        for (int i = 0; i < 4; i++) {
            delete particles[i];
        }
    }
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
    {

        for (int i = 0; i < 4; i++) {
            particles[i]->Update(dt, coObjects);
        }
        ULONGLONG deltaTime = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetDeltaTime(startTime);
        if (deltaTime > BROKEN_BRICK_EFFECT_TIMEOUT) Delete();
    }
    void Render() {
        for (int i = 0; i < 4; i++) {
            particles[i]->Render();
        }
    }
    void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
    }
};

