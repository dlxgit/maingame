#pragma once
#include "declarations.h"
#include "map.h"
#include "inventory.h"

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

void DrawLoot(RenderWindow & window, vector<Loot> & lootList);

bool IsItemInInventory(vector<Loot>::iterator out, vector<Inventory> & inventoryList, Sprite & items);

void GenerateLoot(vector<Loot> & lootList, int ItemsRemaining, NameItem  item, Sprite & texture_items);