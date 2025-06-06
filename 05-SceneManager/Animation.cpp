#include "Animation.h"
#include "debug.h"

void CAnimation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
	if (sprite == NULL)
		DebugOut(L"[ERROR] Sprite ID %d not found!\n", spriteId);

	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}

void CAnimation::Render(float x, float y)
{
	ULONGLONG now = GetTickCount64();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		DWORD adjustedTime = (DWORD)(t / speedScale);
		if (now - lastFrameTime > adjustedTime)
		{
			currentFrame++;
			lastFrameTime = now;

			if (currentFrame == frames.size())
				currentFrame = 0;
		}
	}
	frames[currentFrame]->GetSprite()->Draw(x, y);
}

void CAnimation::RenderCurrentFrame(float x, float y)
{
	lastFrameTime = GetTickCount64();
	if (currentFrame < 0 || currentFrame >= frames.size()) {
		currentFrame = 0;
	}
	frames[currentFrame]->GetSprite()->Draw(x, y);

}

