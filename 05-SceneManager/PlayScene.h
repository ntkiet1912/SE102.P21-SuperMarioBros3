#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "EnemyInfo.h"
//#include "Koopas.h"


class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;					

	vector<LPGAMEOBJECT> objects;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);
	
	// use for spawning enemy when mario's cam is near 
	// or delete enemy when cam is far enough
	vector<CEnemySpawnInfo*> enemySpawns;
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }

	void Clear();
	void PurgeDeletedObjects();
	void AddObject(LPGAMEOBJECT obj) { objects.push_back(obj); }
	void InsertObject(LPGAMEOBJECT obj) { objects.insert(objects.begin() + 1, obj); }

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
	vector<LPGAMEOBJECT>& GetObjects() { return objects; }

};

typedef CPlayScene* LPPLAYSCENE;

