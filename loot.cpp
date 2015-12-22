#pragma once
#include "loot.h"



void DrawLoot(RenderWindow & window, vector<Loot> & lootList)
{
	for (vector<Loot>::iterator i = lootList.begin(); i != lootList.end(); ++i)
	{
		if (i->isDrawn == true)
			window.draw(i->sprite);
	}
};

bool IsItemInInventory(vector<Loot>::iterator out, vector<Inventory> & inventoryList, Sprite & items)
{
	for (std::vector<Inventory>::iterator in = inventoryList.begin(); in != inventoryList.end(); ++in)
	{
		if (in->name == out->name)
		{
			in->quantity += out->quantity;
			in->sprite = items;
			in->sprite.setTextureRect(sf::IntRect(out->name * 32, 0, 32, 32));
			return true;
		}
	}
	return false;
}

void GenerateLoot(vector<Loot> & lootList, vector<Object> & objects, int ItemsRemaining, NameItem  item, Sprite & texture_items)
{
	do
	{
		bool needNewBlock = false;
		float x = (rand() % WIDTH_MAP) * STEP;
		float y = (rand() % HEIGHT_MAP) * STEP;

		FloatRect lootRect = { x,y,texture_items.getGlobalBounds().height,texture_items.getGlobalBounds().height };
		bool isIntersected = false;
		for (size_t i = 0; i < objects.size(); ++i)
		{
			if (lootRect.intersects(objects[i].rect))
			{
				needNewBlock = true;
				break;
			}
		}
		if(!needNewBlock)
		{
			for (vector<Loot>::iterator i = lootList.begin(); i != lootList.end(); ++i)
				if (abs(i->pos.x - x) < 100 && abs(i->pos.y - y) < 100)
				{
					needNewBlock = true;
					break;
				}
			if (needNewBlock == false)
			{
				Loot loot;

				loot.name = item;

				loot.quantity = GetMaxQuantity(loot.name);
				//checkifNeedIt??
				loot.pos = { float(x),float(y) };
				loot.sprite = texture_items;
				loot.sprite.setPosition(loot.pos);
				loot.sprite.setTextureRect(sf::IntRect(item * 32, 0, 32, 32));
				loot.isDrawn = true;

				lootList.push_back(loot);
				ItemsRemaining -= 1;
			}
		}

	} while (ItemsRemaining > 0);
}

void DeleteLoot(vector<Loot> & loot)
{
	for (vector<Loot>::iterator it = loot.begin(); it != loot.end();)
	{
		it = loot.erase(it);
	}
}