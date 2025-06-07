#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "EnemyInfo.h"
#include "BackgroundTile.h"
#include "BlockingWall.h"
//#include "Koopas.h"



class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;					
	ULONGLONG marioPause_start;
	float marioPause_time;
	float preY;
	float gamePause_time;
	float gameResume_time;
	float cmaxX, cminX, cmaxY, cminY;
	bool isMarioPaused = false;
	bool isGamePaused = false;
	bool isGameOver = false;
	bool isFollowing = false;
	float time = 0;
	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> pipes;
	vector<CBackgroundTile*> tiles;
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_TILES(string line);
	void _ParseSection_SETTING(string line);
	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);
	
	// use for spawning enemy when mario's cam is near 
	// or delete enemy when cam is far enough
	vector<CEnemySpawnInfo*> enemySpawns;
	vector<CBlockingWall*> blockingWall;
	//for time countdown
	int timeRemaining = 300;
	float timeAccmulator = 0.0f;
	float timeDecrementInterval = 0.5f;
	float maxCx = 0;
	float maxCy = 0;
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void MarioPause(float time);
	virtual void GamePause();
	virtual void GameResume();
	virtual void GameOver();
	virtual ULONGLONG GetDeltaTime(ULONGLONG start);
	LPGAMEOBJECT GetPlayer() { return player; }

	void Clear();
	void CleanUpDeletedObjects();
	void PurgeDeletedObjects();
	void AddObject(LPGAMEOBJECT obj) { objects.push_back(obj); }
	void InsertObject(LPGAMEOBJECT obj) { objects.insert(objects.begin() + 1, obj); }

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
	vector<LPGAMEOBJECT>& GetObjects() { return objects; }
	void setMaxCx(float x) { maxCx= x; }
	float getMaxCx() { return maxCx; }
	int getID() { return id; }
};

typedef CPlayScene* LPPLAYSCENE;

