#include "inventory.h"

int GetMaxQuantity(NameItem & name)
{
	switch (name)
	{
	case DRINK:
		return 1;
	case PISTOL:
		return 12;
	case RIFLE:
		return 30;
	case AMMO:
		return 1;
	case KEY:
		return 1;
	case MIXTURE:
		return 1;
	case GRENADE:
		return 1;
	case SODA:
		return 10;
	}
}

void InitializeInventory(vector<Inventory> & inventoryList, Sprites & gameSprites)
{
	Inventory inventory;
	
	inventory.name = PISTOL;
	inventory.current = 0;
	inventory.quantity = 7;
	inventory.sprite = gameSprites.items;
	inventory.sprite.setTextureRect(IntRect(32, 0, 32, 32));
	inventoryList.push_back(inventory);
	
	/*
	inventory.name = SODA;
	inventory.current = 0;
	inventory.quantity = 7;
	inventory.sprite = gameSprites.items;
	inventory.sprite.setTextureRect(IntRect(32 * inventory.name, 0, 32, 32));
	inventoryList.push_back(inventory);
	*/
}

void DeleteInventory(vector<Inventory> & inventory)
{
	inventory.clear();
}