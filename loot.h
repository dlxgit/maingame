#pragma once
#include "declarations.h"


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

void DrawLoot(RenderWindow & window, vector<Loot> & lootList)
{
	for (vector<Loot>::iterator i = lootList.begin(); i != lootList.end(); ++i)
	{
		if (i->isDrawn == true)
			window.draw(i->sprite);
	}
};
