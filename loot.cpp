#pragma once
#include "loot.h"

void InitializeLoot(vector<Loot> & lootList, vector<Object> & objects, Sprite & sprite)
{
	GenerateLoot(lootList, objects, 5, DRINK, sprite);
	GenerateLoot(lootList, objects, 3, PISTOL, sprite);
	GenerateLoot(lootList, objects, 2, AMMO, sprite);
	GenerateLoot(lootList, objects, 1, RIFLE, sprite);
	GenerateLoot(lootList, objects, 1, KEY, sprite);
	GenerateLoot(lootList, objects, 1, MIXTURE, sprite);
	GenerateLoot(lootList, objects, 2, GRENADE, sprite);
}


void DrawLoot(RenderWindow & window, vector<Loot> & lootList)
{
	for (Loot & item:lootList)
	{
		if (item.isDrawn == true)
			window.draw(item.sprite);
	}
};

int GetSlotIndexOfItem(Loot & out, vector<Inventory> & inventoryList)
{
	int index = 0;
	for (Inventory & in : inventoryList)
	{
		if (in.name == out.name)
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
		Vector2f newPos = { float((rand() % WIDTH_MAP) * STEP), float((rand() % HEIGHT_MAP) * STEP) };

		FloatRect lootRect = { newPos.x,newPos.y,texture_items.getGlobalBounds().height,texture_items.getGlobalBounds().height };
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
			if (abs(item.pos.x - newPos.x) < 100 && abs(item.pos.y - newPos.y) < 100)
			{
				needNewBlock = true;
				break;
			}
		if (needNewBlock == false)
		{
			Loot loot = GetNewLootItem(item, texture_items, newPos.x, newPos.y);
			lootList.push_back(loot);
			ItemsRemaining -= 1;
		}
	} while (ItemsRemaining > 0);
}

void DeleteLoot(vector<Loot> & loots)
{
	loots.clear();
}