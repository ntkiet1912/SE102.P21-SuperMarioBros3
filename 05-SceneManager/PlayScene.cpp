#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Koopas.h"
#include "Platform.h"
#include "Decoration.h"
#include "Block.h"
#include "Pipe.h"
#include "FirePiranha.h"
#include "LuckyBlock.h"
#include "InvisibleBlock.h"
#include "PlayHUD.h"
#include "DataManager.h"
#include "GoalRoulette.h"
#include "FLyingGround.h"
#include "GoldenBrick.h"
#include "BigPipe.h"
#include "ButtonBrick.h"
#include "WarpPipe.h"
#include "BackgroundTile.h"
#include "DeadZone.h"
#include "GameManager.h"
#include "SampleKeyEventHandler.h"
#include "Piranha.h"
#include "Piranha_Pipe.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define SCENE_SECTION_SETTING 3
#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2
#define ASSETS_SECTION_TILES 3

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}


void CPlayScene::_ParseSection_TILES(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 3) return; // skip invalid lines
	int ID = atoi(tokens[0].c_str());
	int x = atoi(tokens[1].c_str());
	int y = atoi(tokens[2].c_str());
	CBackgroundTile* tile = new CBackgroundTile(CSprites::GetInstance()->Get(ID), x, y);
	tiles.push_back(tile);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			objects.push_back(player);
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	case OBJECT_TYPE_DEAD_ZONE:
		obj = new CDeadZone();
		break;
	case OBJECT_TYPE_PIRANHA:
	{
		if (tokens.size() < 5) return;
		int height = atoi(tokens[3].c_str());
		int pipeHeight = atoi(tokens[4].c_str());
		LPPIRANHAPIPE pipe = new CPiranha_Pipe(
			x, y + height * PIRANHA_CELL_HEIGHT / 2 + PIPE_CELL_HEIGHT / 2, pipeHeight, NULL
		);
		obj = new CPiranha(x, y, height, pipe);
		pipe->piranhaPlant = (LPPIRANHAPLANT)obj;
		y += height * PIRANHA_CELL_HEIGHT;
		break;
	}
	case OBJECT_TYPE_BIG_PIPE:
	{

		if (tokens.size() < 4) return;
		int height = atoi(tokens[3].c_str());
		if (tokens.size() == 5) {
				obj = new CWarpPipe(x, y, height, atoi(tokens[4].c_str()));
		}
		else if (tokens.size() == 4) {
			obj = new CBigPipe(x, y, height);
		}
		else if (tokens.size() == 7) {
			int headId = atoi(tokens[4].c_str());
			int bodyId = atoi(tokens[5].c_str());
			obj = new CWarpPipe(x, y, height, headId, bodyId, atoi(tokens[6].c_str()));
		}
		else {
			int headId = atoi(tokens[4].c_str());
			int bodyId = atoi(tokens[5].c_str());
			obj = new CBigPipe(x, y, height, headId, bodyId);
		}
		pipes.push_back(obj);
		break;
	}
	case OBJECT_TYPE_BUTTON_BRICK: obj = new CButtonBrick(x, y); break;
	case OBJECT_TYPE_GOLDEN_BRICK: obj = new CGoldenBrick(x, y); break;
	case OBJECT_TYPE_GOOMBA:
	{
		obj = new CGoomba(x, y);
		enemySpawns.push_back(new CEnemySpawnInfo(OBJECT_TYPE_GOOMBA, x, y));
		return;
		//break;
	}
	case OBJECT_TYPE_BRICK: obj = new CBrick(x, y); break;
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;
	case OBJECT_TYPE_FIRE_PIRANHA: {
		if (tokens.size() < 4) return;
		int objectID = atoi(tokens[3].c_str());
		obj = new CFirePiranha(x, y, objectID);
		break;
	}
	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}

	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		nextSceneId = scene_id;
		obj = new CPortal(x, y, r, b, scene_id);
		break;
	}
	case OBJECT_TYPE_DECORATION:
	{
		float x = (float)atof(tokens[1].c_str());
		float y = (float)atof(tokens[2].c_str());
		int n = atoi(tokens[3].c_str());

		std::vector<std::pair<float, float>> positions;
		std::vector<int> spriteIDs;

		for (int i = 0; i < n; ++i)
		{
			float px = (float)atof(tokens[4 + i * 2].c_str());
			float py = (float)atof(tokens[5 + i * 2].c_str());
			positions.push_back({ px, py });
		}

		for (int i = 0; i < n; ++i)
		{
			int sid = atoi(tokens[4 + n * 2 + i].c_str());
			spriteIDs.push_back(sid);
		}

		obj = new CDecoration(x, y, positions, spriteIDs);
		break;
	}
	case OBJECT_TYPE_BLOCK:
	{
		float x = (float)atof(tokens[1].c_str());
		float y = (float)atof(tokens[2].c_str());
		float length = (float)atof(tokens[3].c_str());
		float cellWidth = (float)atof(tokens[4].c_str());
		float cellHeight = (float)atof(tokens[5].c_str());

		int n = atoi(tokens[6].c_str());

		std::vector<std::pair<float, float>> positions;
		std::vector<int> spriteIDs;


		for (int i = 0; i < n; ++i)
		{
			float px = (float)atof(tokens[7 + i * 2].c_str());
			float py = (float)atof(tokens[8 + i * 2].c_str());
			positions.push_back({ px, py });
		}

		for (int i = 0; i < n; ++i)
		{
			int sid = atoi(tokens[7 + n * 2 + i].c_str());
			spriteIDs.push_back(sid);
		}

		obj = new CBlock(x, y, length, cellWidth, cellHeight, positions, spriteIDs);
		break;
	}
	case OBJECT_TYPE_INVISIBLE_BLOCK:
	{
		float x = (float)atof(tokens[1].c_str());
		float y = (float)atof(tokens[2].c_str());
		int length = atoi(tokens[3].c_str());
		float cellWidth = (float)atof(tokens[4].c_str());
		float cellHeight = (float)atof(tokens[5].c_str());
		int isBlock = atoi(tokens[6].c_str());
		obj = new CInvisibleBlock(x, y, length, cellWidth, cellHeight, isBlock);
		break;
	}
	case OBJECT_TYPE_KOOPAS:
	{
		int isRed = atoi(tokens[3].c_str());
		int yesWing = atoi(tokens[4].c_str());
		obj = new CKoopas(x, y, isRed, yesWing);
		enemySpawns.push_back(new CEnemySpawnInfo(OBJECT_TYPE_KOOPAS, x, y, isRed, yesWing));
		return;

		break;
	}
	case OBJECT_TYPE_PIPE:
	{
		float x = (float)atof(tokens[1].c_str());
		float y = (float)atof(tokens[2].c_str());
		int length = atoi(tokens[3].c_str());
		float cellWidth = (float)atof(tokens[4].c_str());
		float cellHeight = (float)atof(tokens[5].c_str());
		int n = atoi(tokens[6].c_str());
		std::vector<std::pair<float, float>> positions;
		std::vector<int> spriteIDs;
		for (int i = 0; i < n; ++i)
		{
			float px = (float)atof(tokens[7 + i * 2].c_str());
			float py = (float)atof(tokens[8 + i * 2].c_str());
			positions.push_back({ px, py });
		}
		for (int i = 0; i < n; ++i)
		{
			int sid = atoi(tokens[7 + n * 2 + i].c_str());
			spriteIDs.push_back(sid);
		}
		obj = new CPipe(x, y, length, cellWidth, cellHeight, positions, spriteIDs);
		break;
	}
	case OBJECT_TYPE_LUCKYBLOCK:
	{
		int containItemIndex = atoi(tokens[3].c_str());
		obj = new CLuckyBlock(x, y, containItemIndex);
		break;
	}
	case OBJECT_TYPE_RED_GOOMBA:
	{
		obj = new CRedGoomba(x, y);
		enemySpawns.push_back(new CEnemySpawnInfo(OBJECT_TYPE_RED_GOOMBA, x, y, true, true));
		return;

		break;
	}
	case OBJECT_TYPE_GOAL_ROULETTE_ICON: obj = new CGoalRouletteIcon(x, y); break;
		break;
	case OBJECT_TYPE_FLYINGGROUND:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		//obj = new CFlyingGround(
		//	x, y,
		//	cell_width, cell_height, length,
		//	sprite_begin, sprite_middle, sprite_end
		//);
		enemySpawns.push_back(new CEnemySpawnInfo(OBJECT_TYPE_FLYINGGROUND, x, y, cell_width, cell_height, length, sprite_begin, sprite_middle, sprite_end));
		DebugOut(L"[DONE]\n");
		return;
		break;
	}

	case OBJECT_TYPE_GOLDENLUCKYBLOCK:
	{
		int containItemIndex = atoi(tokens[3].c_str());
		obj = new GoldenLuckyBlock(x, y, containItemIndex);
		break;
	}
	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);


	objects.push_back(obj);
}
void CPlayScene::_ParseSection_SETTING(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 4) return;
	cminX = atoi(tokens[0].c_str());
	cmaxX = atoi(tokens[1].c_str());
	cminY = atoi(tokens[2].c_str());
	cmaxY = atoi(tokens[3].c_str());
}
void CPlayScene::MarioPause(float time)
{
	marioPause_start = GetTickCount64();
	marioPause_time = time;
	isMarioPaused = true;
}

void CPlayScene::GamePause()
{
	gamePause_time = GetTickCount64();
	isGamePaused = true;

}

void CPlayScene::GameResume()
{
	gameResume_time = GetTickCount64();
	isGamePaused = false;
}
void CPlayScene::GameOver()
{
	isGameOver = true;
}

ULONGLONG CPlayScene::GetDeltaTime(ULONGLONG start)
{
	ULONGLONG result = GetTickCount64() - start;
	if (start < marioPause_start) result -= marioPause_time;
	if (start < gameResume_time) {
		if (start > gamePause_time) result -= (gameResume_time - start);
		else result -= (gameResume_time - gamePause_time);;
	}
	return result;
}


void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line == "[TILES]") { section = ASSETS_SECTION_TILES; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case ASSETS_SECTION_TILES: _ParseSection_TILES(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	timeRemaining = 300;
	timeAccmulator = 0.0f;

	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);
	player = CGame::GetInstance()->GetPlayer();
	ifstream f;
	f.open(sceneFilePath);
	switch (id)
	{
	case 6:
		maxCx = 2885;
		maxCy = -50;
		break;
	case 4: 
		maxCx = 1900;
		break;
	default:
		maxCx = 2885;
		break;
	}
	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line == "[SETTING]") { section = SCENE_SECTION_SETTING; continue; };
		
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_SETTING: _ParseSection_SETTING(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
	time = CGame::GetInstance()->GetGameTime();
}

// considered is one object inside the camera ? 
bool isInCamera(float x, float y, float width = 50.0f, float height = 80.0f)
{
	float camX, camY;
	CGame::GetInstance()->GetCamPos(camX, camY);
	float screenW = CGame::GetInstance()->GetBackBufferWidth();
	float screenH = CGame::GetInstance()->GetBackBufferHeight();

	return (x > camX - width && x < camX + screenW + width && y < camY + screenH + height && y > camY - height);
}
void CPlayScene::Update(DWORD dt)
{

	// Cập nhật HUD
	CPlayHUD::GetInstance()->SetTime(timeRemaining);
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	vector<LPGAMEOBJECT> coObjects;

	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	/*
		Spawning enemies has 2 scenerios : IN and OUT of player Camera
			+ IN : if objects is spawned: keep updating
				 : if not spawn yet, give it a chance to appear
			+ OUT: keep the original info, except the instance
		-> delete the "outcamera" object, set its instance to nullptr


		we can't update or render enemySpawns because main idea is based on vector objects
		-> so when an object in enemySpawns activated, it needs to be added to objects
	*/
	for (size_t i = 0; i < enemySpawns.size(); i++)
	{
		float x = enemySpawns[i]->x;
		float y = enemySpawns[i]->y;
		float preSpawnX = 100.0f;
		if (id == 4)
		{
			preSpawnX = 0;
		}
		if (isInCamera(x, y, preSpawnX))
		{
			if (!enemySpawns[i]->isSpawned)
			{
				LPGAMEOBJECT enemy = nullptr;
				switch (enemySpawns[i]->type)
				{
				case OBJECT_TYPE_KOOPAS:
					enemy = new CKoopas(x, y, enemySpawns[i]->isRed, enemySpawns[i]->yesWing);
					break;
				case OBJECT_TYPE_GOOMBA:
					enemy = new CGoomba(x, y);
					break;
				case OBJECT_TYPE_RED_GOOMBA:
					enemy = new CRedGoomba(x, y);
					break;
				case OBJECT_TYPE_FLYINGGROUND:
						enemy = new CFlyingGround(
						x, y,
						enemySpawns[i]->cellWidth, enemySpawns[i]->cellHeight, enemySpawns[i]->length,
							enemySpawns[i]->spriteIdBegin, enemySpawns[i]->spriteIdMiddle, enemySpawns[i]->spriteIdEnd
					);
						break;
				}
				enemySpawns[i]->instance = enemy;
				enemySpawns[i]->isSpawned = true;
				coObjects.push_back(enemy);
				objects.push_back(enemy);
			}
		}
		// can't use else because x and y is original pos of enemy 
		// when it reached bounds like cam + screenWidth + offset, and even in camera, 
		// it still disappeared. 
		else if (!isInCamera(x, y, 500))
		{
			enemySpawns[i]->isSpawned = false;

			/*
				i dont use enemySpawns[i]->instance->Delete()
				because it causes Exception error ... bla bla
					-> so delete by using std::remove is a better solution
			*/
			objects.erase(std::remove(objects.begin(), objects.end(), enemySpawns[i]->instance), objects.end());
			coObjects.erase(std::remove(coObjects.begin(), coObjects.end(), enemySpawns[i]->instance), coObjects.end());
		}
	}
	// when enemySpawns updated: this' time for objects 
	for (size_t i = 0; i < objects.size(); i++)
	{
		//if (dynamic_cast<CMario*>(objects[i])) continue;
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	CGame* game = CGame::GetInstance();
	player->GetPosition(cx, cy);

	//if (id == 4)
	//{
	//	static bool isLoadWall = false;
	//	static float autoCamX = 0.0f;
	//	autoCamX += 0.02f * dt;
	//	float minCamX = -20;
	//	float maxCamX = cx + game->GetBackBufferWidth() / 1.75;
	//	//cy -= game->GetBackBufferHeight() / 2;
	//	if (!isLoadWall)
	//	{
	//		for (int i = -5; i < 15; i++)
	//		{
	//			CBlockingWall* wall = new CBlockingWall(minCamX, 17 * i);
	//			AddObject(wall);
	//			blockingWall.push_back(wall);

	//			wall = new CBlockingWall(maxCamX, 17 * i);
	//			AddObject(wall);
	//			blockingWall.push_back(wall);
	//		}
	//		isLoadWall = true;
	//	}
	//	for (size_t i = 0; i < blockingWall.size(); i++)
	//	{
	//		float x, y;
	//		blockingWall[i]->GetPosition(x, y);
	//		if (i % 2 == 0)
	//			blockingWall[i]->SetPosition(autoCamX + (0.02f * dt), y);
	//		else
	//		{
	//			blockingWall[i]->SetPosition(x + (0.02f * dt), y);
	//		}
	//	}
	//	if (autoCamX + game->GetBackBufferWidth() / 1.5 > cmaxX) autoCamX = cmaxX - game->GetBackBufferWidth() / 1.5;

	//	CGame::GetInstance()->SetCamPos(autoCamX, -50.0f);
	//}
	//else if (id == 6)
	//{
	//	float px, py;
	//	player->GetPosition(cx, cy);
	//	player->GetPosition(px, py);
	//	cx -= game->GetBackBufferWidth() / 2;
	//	cy -= game->GetBackBufferHeight() / 2;
	//	if (cx + game->GetBackBufferWidth() / 1.5 > maxCx)  cx = maxCx - game->GetBackBufferWidth() / 1.5;
	//	float targetCamY = 0;
	//	if (py < 40)
	//	{
	//		targetCamY = py - game->GetBackBufferHeight() / 2;
	//		if (targetCamY < -200.0f) targetCamY = -200.0f;
	//		if (targetCamY > 0.0f) targetCamY = 0.0f;
	//	}
	//	else
	//	{
	//		cy = 0;
	//	}
	//	if (cy < -200) cy = -200;
	//	cy = cy * 0.9f + targetCamY * 0.1f;

	//	CGame::GetInstance()->SetCamPos(cx, cy);
	//}
	//else
	{
		float px, py;
		player->GetPosition(cx, cy);
		player->GetPosition(px, py);
		cx -= game->GetBackBufferWidth() / 2;
		cy -= game->GetBackBufferHeight() / 2;
		maxCx = 2980;
		if (cx + game->GetBackBufferWidth() / 1.5 > maxCx)  cx = maxCx - game->GetBackBufferWidth() / 1.5;
		CGame::GetInstance()->SetCamPos(cx, 0);
	}
	


	PurgeDeletedObjects();

	// Update time
	timeAccmulator += dt / 1000.0f;
	if (timeAccmulator >= timeDecrementInterval)
	{
		timeRemaining--;
		timeAccmulator = 0.0f;

		if (timeRemaining <= 0)
		{

			player->SetState(MARIO_STATE_DIE);
			timeRemaining = 0;
			return;
		}
	}

	CGameManager::GetInstance()->Update(dt);
	// Cập nhật HUD
	CPlayHUD::GetInstance()->SetTime(timeRemaining);

}
void CPlayScene::CleanUpDeletedObjects()
{
	for (auto it = objects.begin(); it != objects.end(); )
	{
		if ((*it)->IsDeleted()) // hàm getter bạn cần cài thêm
		{
			delete* it;
			it = objects.erase(it);
		}
		else
			++it;
	}
}
void CPlayScene::Render()
{
	for (int i = 0; i < tiles.size(); i++)
		tiles[i]->Draw();
	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<CMario*>(objects[i])) continue;
		objects[i]->Render();
	}
	for (int i = 0; i < pipes.size(); i++) {
			pipes[i]->Render();
	}
	player->Render();
	CGameManager::GetInstance()->Render();
	CPlayHUD::GetInstance()->Render();
}




/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();

}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well

*/
void CPlayScene::Unload()
{
	for (int i = 1; i < objects.size(); i++)
		delete objects[i];

	for (int i = 0; i < tiles.size(); i++) 
		delete tiles[i];
		tiles.clear();

	pipes.clear();
	objects.clear();
	CGame::GetInstance()->SetPlayer(player);
	player = NULL;

	CPlayHUD::GetInstance()->Clear();
	 CGame::GetInstance()->SetGameTime(time);

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}