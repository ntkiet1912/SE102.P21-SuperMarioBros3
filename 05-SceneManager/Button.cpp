#include "Button.h"
#include "GoldenBrick.h"

void CButton::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == BUTTON_STATE_NORMAL)
	{
		l = x - BRICK_BBOX_WIDTH / 2;
		t = y - BRICK_BBOX_HEIGHT / 2;
		r = l + BRICK_BBOX_WIDTH;
		b = t + BRICK_BBOX_HEIGHT;
	}
	else l = t = r = b = 0;

}
void CButton::Render()
{
	if (state == BUTTON_STATE_NORMAL) {
		CAnimations::GetInstance()->Get(ID_ANI_BUTTON)->Render(x, y);
	}
	else if (state == BUTTON_STATE_PRESSED) {
		CAnimations::GetInstance()->Get(ID_ANI_BUTTON_PRESSED)->Render(x, y);
	}
}

void CButton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (handled) return;
	if (state == BUTTON_STATE_PRESSED) {
		for (int i = 0; i < coObjects->size(); i++) {
			if (dynamic_cast<CGoldenBrick*>(coObjects->at(i))) {
				CGoldenBrick* goldenBrick = dynamic_cast<CGoldenBrick*>(coObjects->at(i));
				goldenBrick->SetState(GOLDEN_BRICK_STATE_GOLD);
			}
		}
		handled = true;
	}
}

int CButton::IsBlocking()
{
	if (state == BUTTON_STATE_NORMAL) return 1;
	return 0;
}


