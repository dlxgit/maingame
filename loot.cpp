#pragma once
#include "loot.h"


void DrawLoot(RenderWindow & window, vector<Loot> & lootList)
{
	for (Loot & item:lootList)
	{
		if (item.isDrawn == true)
			window.draw(item.sprite);
	}
};

int GetSlotIndexOfItem(vector<Loot>::iterator out, vector<Inventory> & inventoryList)
{
	int index = 0;
	for (std::vector<Inventory>::iterator in = inventoryList.begin(); in != inventoryList.end(); ++in)
	{
		if (in->name == out->name)
		{
			return index;
		}
		index++;
	}
	return -1;
}

Loot GetNewLootItem(NameItem & item, Sprite & sprite, float & x, float & y)
{
	Loot loot;
	loot.name = item;
	loot.quantity = GetMaxQuantity(loot.name);
	loot.pos = {x,y};
	loot.sprite = sprite;
	loot.sprite.setPosition(loot.pos);
	loot.sprite.setTextureRect(sf::IntRect(item * 32, 0, 32, 32));
	loot.isDrawn = true;
	return loot;
}

void GenerateLoot(vector<Loot> & lootList, vector<Object> & objects, int ItemsRemaining, NameItem  item, Sprite & texture_items)
{
	//TODO: MAKE FUNCTION MORE READABLE
	do
	{
		bool needNewBlock = false;
		float x = (rand() % WIDTH_MAP) * STEP;
		float y = (rand() % HEIGHT_MAP) * STEP;

		FloatRect lootRect = { x,y,texture_items.getGlobalBounds().height,texture_items.getGlobalBounds().height };
		bool isIntersected = false;
		for (size_t i = 0; i < objects.size() &&  needNewBlock == false; ++i)
		{
			if (lootRect.intersects(objects[i].rect))
			{
				needNewBlock = true;
			}
		}
		if (needNewBlock)
		{
			continue;
		}
		for (Loot & item : lootList)
			if (abs(item.pos.x - x) < 100 && abs(item.pos.y - y) < 100)
			{
				needNewBlock = true;
				break;
			}
		if (needNewBlock == false)
		{
			Loot loot = GetNewLootItem(item, texture_items, x, y);
			lootList.push_back(loot);
			ItemsRemaining -= 1;
		}
	} while (ItemsRemaining > 0);
}

void DeleteLoot(vector<Loot> & loots)
{
	loots.clear();
}