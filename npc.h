#pragma once
#include <vector>
#include "declarations.h"
#include "map.h"
#include "hero.h"

using namespace sf;
using namespace std;

struct Npc
{
	Vector2f pos;
	Texture texture;
	Sprite sprite;
	int health;
	NpcState state;
	float currentFrame;
	NpcType type;
};


void InitializeNpc(vector<Npc> & npcList, Sprite & sprite_npc);

void ComputeNpcFrame(vector<Npc> & npcList);

void CheckEventNpc(vector<Npc> & npcList, Hero & hero);