#include "SCorePopup.h"
#include "Game.h"
ScorePopupManager* ScorePopupManager::__instance = nullptr;

ScorePopupManager::ScorePopupManager() {
	InitScorePopupPool();
}

ScorePopupManager* ScorePopupManager::GetInstance() {
	if (!__instance) {
		__instance = new ScorePopupManager();
	}
	return __instance;
}

void ScorePopupManager::InitScorePopupPool() {
	scorePopupPool.clear();
	for (int i = 0; i < MAX_POOL_SIZE; ++i) {
		ScorePopup popup;
		popup.active = false;
		scorePopupPool.push_back(popup);
	}
}



void ScorePopupManager::SpawnScorePopup(const Vector2& position, const std::string& scoreText) {
	for (auto& popup : scorePopupPool) {
		if (!popup.active) {
			popup.position = position;
			popup.text = scoreText;
			popup.alpha = 1.0f;
			popup.lifetime = 0.0f;
			popup.active = true;
			break;
		}
	}
}

void ScorePopupManager::UpdateScorePopup(float dt) {
	for (auto& popup : scorePopupPool) {
		if (!popup.active) continue;

		popup.lifetime += dt;
		popup.position.y -= 30.0f * dt; 
		popup.alpha = 1.0f - (popup.lifetime / 1.0f); 

		if (popup.lifetime >= 1.0f) {
			popup.active = false;
		}
	}
}

void ScorePopupManager::RenderScorePopup(ID3DX10Font* font) {
	for (const auto& popup : scorePopupPool) {
		if (!popup.active) continue;

		D3DXCOLOR color(1.0f, 1.0f, 0.0f, popup.alpha);
		RECT rect;
		float x, y;
		CGame::GetInstance()->GetCamPos(x,y);
		rect.left = (LONG)(popup.position.x - x);
		rect.top = (LONG)(popup.position.y - y);
		rect.right = rect.left + 100;
		rect.bottom = rect.top + 30;

		font->DrawTextA(NULL, popup.text.c_str(), -1, &rect, DT_NOCLIP, color);
	}
}