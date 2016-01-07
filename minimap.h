#pragma once
#define _USE_MATH_DEFINES
#include "declarations.h"
#include "npc.h"
#include "sprites.h"
#include <math.h>

using namespace sf;
using namespace std;

struct MiniMap
{
	CircleShape circle;
	Sprite heroDot;
	vector<Sprite> npcDotList;
};

void ComputeNpcDotDegree(Sprite & hero, Sprite & npc);

void InitializeMinimap(MiniMap & miniMap, vector<Npc> & npcList, Sprites & sprites);

void UpdateMinimap(MiniMap & miniMap, vector<Npc> & npcList, Sprite & hero, View & view);

void DrawMiniMap(RenderWindow & window, MiniMap & miniMap);

void DeleteNpcDot(MiniMap & miniMap, int & index);