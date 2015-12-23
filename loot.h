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

void DrawLoot(RenderWindow & window, vector<Loot> & lootList);

bool IsItemInInventory(vector<Loot>::iterator out, vector<Inventory> & inventoryList, Sprite & items);

Loot GetNewLootItem(NameItem & item, Sprite & sprite, float & x, float & y);

void GenerateLoot(vector<Loot> & lootList, vector<Object> & objects, int ItemsRemaining, NameItem  item, Sprite & texture_items);

void DeleteLoot(vector<Loot> & loot);