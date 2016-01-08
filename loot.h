#pragma once
#include "declarations.h"
#include "inventory.h"
#include "level.h"

using namespace std;

struct Loot
{
	NameItem name;
	int quantity;
	Vector2f pos;
	Texture texture;
	Sprite sprite;
	bool isDrawn;
};

void InitializeLoot(vector<Loot> & lootList, vector<Object> & objects, Sprite & sprite);

void DrawLoot(RenderWindow & window, vector<Loot> & lootList);

int GetSlotIndexOfItem(Loot & out, vector<Inventory> & inventoryList);

Loot GetNewLootItem(NameItem & item, Sprite & sprite, float & x, float & y);

void GenerateLoot(vector<Loot> & lootList, vector<Object> & objects, int ItemsRemaining, NameItem  item, Sprite & texture_items);

void DeleteLoot(vector<Loot> & loot);