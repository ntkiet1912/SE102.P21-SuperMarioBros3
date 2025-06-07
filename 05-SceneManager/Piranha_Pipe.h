#pragma once
#include "BigPipe.h"
#include "Piranha.h"
class CPiranha_Pipe :
	public CBigPipe
{
public:
	LPPIRANHAPLANT piranhaPlant;
	CPiranha_Pipe(float x, float y, int height, LPPIRANHAPLANT piranha) :CBigPipe(x, y, height)
	{
		this->piranhaPlant = piranha;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
};
typedef CPiranha_Pipe* LPPIRANHAPIPE;

