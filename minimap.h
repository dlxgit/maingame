#pragma once
#define _USE_MATH_DEFINES
#include "declarations.h"
#include "npc.h"
#include <math.h>

using namespace sf;
using namespace std;

struct MiniMap
{
	CircleShape circle;
	CircleShape heroDot;
	vector<CircleShape> npcDotList;
};

void ComputeNpcDotDegree(Sprite & hero, Sprite & npc);

void InitializeMinimap(MiniMap & miniMap, vector<Npc> & npcList);

void UpdateMinimap(MiniMap & miniMap, vector<Npc> & npcList, Sprite & hero);

void DrawMiniMap(RenderWindow & window, MiniMap & miniMap);