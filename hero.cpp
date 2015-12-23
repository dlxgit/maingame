#pragma once
#include <SFML/Graphics.hpp>
#include "declarations.h"
#include "hero.h"


void InitializeHero(Hero & hero)
{
	hero.slotNo = 0;
	hero.nSlots = 1;
	hero.health = 100;
	hero.texture.loadFromFile("resources/images/hero.png");
	hero.sprite.setTexture(hero.texture);
	hero.sprite.setTextureRect(IntRect(4, 4, 32, 32));
	hero.dir = NONE;
	hero.dirLast = DOWN;
	hero.currentFrame = 0;
	hero.sprite.setPosition(6 * 32, 9 * 32);  //start position
	hero.state = NORMAL;
	hero.isBeastAttack = false;
	hero.isWeaponSwitch = false;
	hero.lastAttackTime = 0;
	hero.lastReloadTime = 0;
	hero.savedNeighbors = 0;
};

void CheckGameOver(GameState & state, Hero & hero)
{
	if (hero.health <= 0)
	{
		state = END_GAME;
	}
	if (hero.savedNeighbors >= MAX_NUMBER_OF_NEIGHBORS)
	{
		state = LEVEL_FINISH;
	}
}


void UpdateDirection(Hero & hero)
{
	//update hero direction
	if (Keyboard::isKeyPressed(Keyboard::Up))
	{
		if (Keyboard::isKeyPressed(Keyboard::Right))  hero.dir = UPRIGHT;
		else if (Keyboard::isKeyPressed(Keyboard::Left))  hero.dir = UPLEFT;
		else hero.dir = UP;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Down))
	{
		if (Keyboard::isKeyPressed(Keyboard::Left)) hero.dir = DOWNLEFT;
		else if (Keyboard::isKeyPressed(Keyboard::Right)) hero.dir = DOWNRIGHT;
		else hero.dir = DOWN;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Left))
	{
		if (Keyboard::isKeyPressed(Keyboard::Down)) hero.dir = DOWNLEFT;
		else hero.dir = LEFT;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		if (Keyboard::isKeyPressed(Keyboard::Down)) hero.dir = DOWNRIGHT;
		else hero.dir = RIGHT;
	}
	else hero.dir = NONE;
};

bool IsInventorySwitch(Hero & hero)
{
	bool isSwitch = false;
	if (hero.isWeaponSwitch == false)
	{
		if (Keyboard::isKeyPressed(Keyboard::X))
		{
			hero.isWeaponSwitch = true;
			hero.slotNo += 1;
			if (hero.slotNo >= hero.nSlots) hero.slotNo = 0;
			isSwitch = true;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Z))
		{
			hero.isWeaponSwitch = true;
			hero.slotNo -= 1;
			if (hero.slotNo < 0) hero.slotNo = hero.nSlots - 1;
			isSwitch = true;
		}
	}
	else if ((Keyboard::isKeyPressed(Keyboard::X) || Keyboard::isKeyPressed(Keyboard::Z)) == false)
		hero.isWeaponSwitch = false;
	return isSwitch;
};


bool UpdateInventory(Hero & hero, vector<Inventory> & inventoryList, float & time)
{
	//update items 
	if (inventoryList[hero.slotNo].current == 0 && time - hero.lastReloadTime > WEAPON_RELOAD_TIME)
	{
		hero.lastReloadTime = time;
		hero.isReloading = false;
		if (inventoryList[hero.slotNo].quantity >= MAX_AMMO[inventoryList[hero.slotNo].name])
		{
			inventoryList[hero.slotNo].quantity -= MAX_AMMO[inventoryList[hero.slotNo].name];
			inventoryList[hero.slotNo].current = MAX_AMMO[inventoryList[hero.slotNo].name];
		}
		else
		{
			inventoryList[hero.slotNo].current = inventoryList[hero.slotNo].quantity;
			inventoryList[hero.slotNo].quantity = 0;
		}
		if (inventoryList[hero.slotNo].quantity <= 0)
		{
			inventoryList[hero.slotNo].quantity = 0;
		}
		return true;

	}
};

void CheckUsingItems(Hero & hero, vector<Inventory> & inventoryList, vector<Shot> & shotList, float & time, Sprite & sprite_shot, Sprite & sprite_grenade)
{
	if (Keyboard::isKeyPressed(Keyboard::A))
	{
		if (hero.state == BEAST)
		{
			if (time - hero.lastAttackTime > HERO_BEAST_ATTACK_TIME)
			{
				//hero.lastAttackTime = game.time;  (reminder)
				hero.isBeastAttack = true;
				hero.currentFrame = 0;
			}
		}
		else if (hero.state == NORMAL && inventoryList[hero.slotNo].current > 0)
		{
			if (time > hero.shotLastTime + ITEM_REUSE_COOLDOWN[inventoryList[hero.slotNo].name])
			{
				hero.shotLastTime = time;
				inventoryList[hero.slotNo].current -= 1;
				if (inventoryList[hero.slotNo].name == PISTOL || inventoryList[hero.slotNo].name == RIFLE)
				{
					AddNewShot(shotList, hero.dirLast, hero.pos, time, sprite_shot, sprite_grenade, BULLET);
				}
				else if (inventoryList[hero.slotNo].name == DRINK)
				{
					hero.health += HP_PER_DRINK;
					if (hero.health > 100)
					{
						hero.health = 100;
					}
				}
				else if (inventoryList[hero.slotNo].name == MIXTURE)
				{
					hero.state = TRANSFORMING;
					hero.dir = NONE;
					hero.dirLast = DOWN;
				}
				else if (inventoryList[hero.slotNo].name == GRENADE)
				{
					AddNewShot(shotList, hero.dirLast, hero.pos, time, sprite_shot, sprite_grenade, USED_GRENADE);
				}
			}
		}
	}
};


void UpdateHeroSprite(Hero & hero, float & time)
{
	if (hero.state == TRANSFORMING)
	{
		hero.sprite.setTextureRect(IntRect(11 + 37 * int(hero.currentFrame), 484, 32, 45));
		hero.currentFrame += 0.05;

		if (hero.currentFrame > 7)
		{
			hero.state = BEAST;
			hero.beastTimer = time;
			hero.currentFrame = 0;
		}
	}
	else if (hero.state == BEAST)
	{
		if (hero.isBeastAttack)  //attacking beast animation
		{
			switch (hero.dirLast)
			{
			case UP:
				hero.sprite.setTextureRect(IntRect(355 + 40 * int(hero.currentFrame), 597, 34, 47));
				break;
			case UPRIGHT: case RIGHT: case DOWNRIGHT:
				//21 38
				hero.sprite.setTextureRect(IntRect(182 + 57 * int(hero.currentFrame), 598, 48, 52));
				break;
			case DOWN:
				hero.sprite.setTextureRect(IntRect(42 + 35 * int(hero.currentFrame), 595, 35, 54));
				break;
			case DOWNLEFT: case LEFT: case UPLEFT:
				hero.sprite.setTextureRect(IntRect(492 + 56 * int(hero.currentFrame), 601, 54, 52));
				break;
			case NONE:
				break;
			}

			hero.currentFrame += 0.2f;
			if (hero.currentFrame > 2)
			{
				//TODO: deal damage after this
				hero.isBeastAttack = false;
				hero.currentFrame = 0;
			}
		}
		else  //moving beast animation
		{
			switch (hero.dir)
			{
			case UP:
				hero.sprite.setTextureRect(IntRect(326 + 40 * int(hero.currentFrame), 537, 37, 47));
				break;
			case UPRIGHT: case RIGHT: case DOWNRIGHT:
				//21 38
				hero.sprite.setTextureRect(IntRect(163 + 40 * int(hero.currentFrame), 537, 37, 47));
				break;
			case DOWN:
				hero.sprite.setTextureRect(IntRect(9 + 38 * int(hero.currentFrame), 537, 37, 47));
				break;
			case DOWNLEFT: case LEFT: case UPLEFT:
				hero.sprite.setTextureRect(IntRect(480 + 40 * int(hero.currentFrame), 537, 37, 47));
				break;
			case NONE:
				if (hero.dirLast == UP)
				{
					hero.sprite.setTextureRect(IntRect(270 + 76, 485, 36, 46));
				}
				else if (hero.dirLast == RIGHT)
				{
					hero.sprite.setTextureRect(IntRect(270 + 39, 485, 36, 46));
				}
				else if (hero.dirLast == DOWN)
				{
					hero.sprite.setTextureRect(IntRect(270, 485, 36, 46));
				}
				else if (hero.dirLast == LEFT)
				{
					hero.sprite.setTextureRect(IntRect(270 + 114, 485, 36, 46));
				}
				break;
			}

			hero.currentFrame += 0.2;

			if (hero.currentFrame > 3)
			{
				cout << hero.currentFrame << " FRAME" << endl;
				hero.currentFrame = 0;

			}
		}
	}
	else if (hero.state == NORMAL)  //normal moving animation
	{
		switch (hero.dir)
		{
		case UP:
			hero.sprite.setTextureRect(IntRect(105 + 22 * int(hero.currentFrame), 84, 21, 37));
			break;
		case UPRIGHT: case RIGHT: case DOWNRIGHT:
			//21 38
			hero.sprite.setTextureRect(IntRect(105 + 22 * int(hero.currentFrame), 44, 21, 37));
			break;
		case DOWN:
			hero.sprite.setTextureRect(IntRect(105 + 22 * int(hero.currentFrame), 4, 21, 37));
			break;
		case DOWNLEFT: case LEFT: case UPLEFT:
			hero.sprite.setTextureRect(IntRect(105 + 22 * int(hero.currentFrame), 124, 21, 37));
			break;
		case NONE:
			if (hero.dirLast == UP)
			{
				hero.sprite.setTextureRect(IntRect(86, 85, 21, 36));
			}
			else if (hero.dirLast == RIGHT)
			{
				hero.sprite.setTextureRect(IntRect(86, 45, 21, 36));
			}
			else if (hero.dirLast == DOWN)
			{
				hero.sprite.setTextureRect(IntRect(86, 5, 21, 36));
			}
			else if (hero.dirLast == LEFT)
			{
				hero.sprite.setTextureRect(IntRect(86, 125, 21, 36));
			}
			break;
		}

		hero.currentFrame += 0.2f;
		if (hero.currentFrame > 4)
		{
			hero.currentFrame = 0;
		}
	}
	else if (hero.state == DAMAGED)
	{
		hero.sprite.setTextureRect(IntRect(10 + 32 * int(hero.currentFrame), 179, 32, 45));
		hero.currentFrame += 0.06;
		if (hero.currentFrame > 2)
		{
			hero.currentFrame = 0;
			hero.state = NORMAL;
		}
	}
	if (hero.dir != NONE)
	{
		hero.dirLast = hero.dir;  //update dirLast (for shooting)
		if (hero.dir != hero.dirLast)
		{
			hero.currentFrame = 0;
		}
	}
	else
	{
		hero.currentFrame = 0;
	}
};

void DrawHero(RenderWindow & window, Hero & hero)
{
	window.draw(hero.sprite);
};

void DrawInventoryText(RenderWindow & window, vector<Inventory> & inventoryList, Hero & hero, View & view, Text & text)
{
	Vector2f posView = GetInterfacePosition(view);  //zametka

	text.setPosition(posView.x + 40, posView.y + 40);

	//currentItem << (hero.iCurrent.item);
	std::ostringstream toStringCurrent;
	toStringCurrent << inventoryList[hero.slotNo].current;

	std::ostringstream toStringQuantity;
	toStringQuantity << inventoryList[hero.slotNo].quantity;
	text.setString(toStringCurrent.str() + "/" + toStringQuantity.str() + " " + ITEM_NAMES[inventoryList[hero.slotNo].name]);
	window.draw(text);
	if (hero.isReloading)
	{
		text.setPosition(posView.x + 40, posView.y + 70);
		if (inventoryList[hero.slotNo].name == PISTOL || inventoryList[hero.slotNo].name == RIFLE)
		{
			text.setString("reloading");
		}
		else if (inventoryList[hero.slotNo].name == GRENADE)
		{
			//text.setString("pulling");
		}
		else
		{
			//text.setString("opening");
		}
		window.draw(text);
	}
	std::ostringstream toStringNeighbors;
	toStringNeighbors << hero.savedNeighbors;
	text.setString("rescued: " + toStringNeighbors.str());
	text.setPosition(posView.x + 5, posView.y + 100);
	window.draw(text);

	std::ostringstream toStringRemaining;
	int remaining = MAX_NUMBER_OF_NEIGHBORS - hero.savedNeighbors;
	toStringRemaining << remaining;
	text.setString("remaining: " + toStringRemaining.str());
	text.setPosition(posView.x + 5, posView.y + 120);
	window.draw(text);
}

Inventory GetNewInventoryItem(Loot & loot, Sprite & items)
{
	Inventory inventory;
	inventory.name = loot.name;
	inventory.quantity = loot.quantity;
	inventory.current = 0;
	inventory.sprite = items;
	inventory.sprite.setTextureRect(sf::IntRect(loot.name * 32, 0, 32, 32));
	return inventory;
}

void CheckLoot(Hero & hero, vector<Loot> & lootList, vector<Inventory> & inventoryList, Sprite & items)
{
	float x = hero.sprite.getPosition().x;
	float y = hero.sprite.getPosition().y;

	bool isItemAlreadyIn = false;

	for (vector<Loot>::iterator out = lootList.begin(); out != lootList.end();)
	{
		if (out->isDrawn == true)  //if loot.item.center contains heroSprite  -> add new item in inventory
		{
			Vector2f itemCenter;
			itemCenter.x = out->pos.x + out->sprite.getGlobalBounds().width / 2;
			itemCenter.y = out->pos.y + out->sprite.getGlobalBounds().height / 2;
			if (hero.sprite.getGlobalBounds().contains(itemCenter))
			{
				if (out->name != AMMO)  //any item that we can take
				{
					//check if this item exists in inventory, and if so - upload it
					if (IsItemInInventory(out, inventoryList, items))
					{
						inventoryList[hero.slotNo].quantity += out->quantity;
						inventoryList[hero.slotNo].sprite = items;
						inventoryList[hero.slotNo].sprite.setTextureRect(sf::IntRect(out->name * 32, 0, 32, 32));
						out->isDrawn = false;
						isItemAlreadyIn = true;
					}
					if (isItemAlreadyIn == false) //adding new item to inventory List
					{
						Inventory inventory = GetNewInventoryItem(*out, items);
						inventoryList.push_back(inventory);

						out->isDrawn = false;
						isItemAlreadyIn = true;
						hero.nSlots += 1;
					}
				}
				else
				{
					out->isDrawn = false;
					int nWeaponAmmoAdded = 0;
					while (nWeaponAmmoAdded < AMMO_PACKS)
					{
						//delSoon>??
						for (Inventory itm : inventoryList)
						{
							if (itm.name != MIXTURE && itm.name != KEY && itm.name != DRINK)
							{
								itm.quantity += MAX_AMMO[itm.name];
								nWeaponAmmoAdded += 1;
							}
						}
						break;
					}
				}
			}
			out++;
		}
		else
		{
			out = lootList.erase(out);
		}
	}
}