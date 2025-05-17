#pragma once  
#include <string>
#include <D3DX10core.h>
#include <vector>

struct Vector2 {
    float x, y;
    Vector2(float _x = 0.0f, float _y = 0.0f) : x(_x), y(_y) {}
};

struct ScorePopup  
{  
public:
   Vector2 position; 
   float alpha;
   float lifetime;
   std::string text;
   bool active;

   ScorePopup()
	   : position(0.0f , 0.0f), lifetime(0.0f),alpha(1.0f), active(false) {
   }

};

class ScorePopupManager {
private:
	static ScorePopupManager* __instance;
	std::vector<ScorePopup> scorePopupPool;
	const int MAX_POOL_SIZE = 50;

	ScorePopupManager();
public:
	static ScorePopupManager* GetInstance();
	void InitScorePopupPool();
	void SpawnScorePopup(const Vector2& position, const std::string& scoreText);
	void UpdateScorePopup(float dt);
	void RenderScorePopup(ID3DX10Font* font);
};
