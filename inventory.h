#pragma once
#include <vector>
#include "SFML\Graphics.hpp"
#include "sprites.h"
#include "declarations.h"

using namespace sf;
using namespace std;



struct Inventory
{
	NameItem name;
	int quantity;
	int current;
	Texture texture;
	Sprite sprite;
};

struct Grenade
{
	Vector2f startPos;
	Vector2f distance;
	float startTime;
	bool isExploded;
};

int GetMaxQuantity(NameItem & name);

void InitializeInventory(vector<Inventory> & inventoryList, Sprites & gameSprites);

void DeleteInventory(vector<Inventory> & inventory);