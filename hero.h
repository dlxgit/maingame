#pragma once
#include <SFML/Graphics.hpp>
#include "declarations.h"
#include "loot.h"
#include "shot.h"
#include "view.h"
#include "sprites.h"
#include <sstream>
#include "level.h"

using namespace sf;

struct Hero
{
	Vector2f pos;
	Texture texture;
	Sprite sprite;
	Direction dir;
	Direction dirLast;
	int slotNo;
	int nSlots;		//number of slots in inventory
	int health;
	float currentFrame;
	HeroState state;
	bool isBeastAttack;
	float beastTimer;
	float lastAttackTime;
	bool isAmmo;
	float lastReloadTime;
	bool isReloading;
	int savedNeighbors;
	bool isWeaponSwitch;
	float shotLastTime;
	Inventory item;
};

void InitializeHero(Hero & hero);

void CheckGameOver(GameState & state, Hero & hero);

void UpdateDirection(Hero & hero);

bool IsInventorySwitch(Hero & hero);

bool UpdateInventory(Hero & hero, vector<Inventory> & inventoryList, float & time);

Inventory GetNewInventoryItem(Loot & loot, Sprite & items);

void CheckUsingItems(Hero & hero, vector<Inventory> & inventoryList, vector<Shot> & shotList, float & time, Sprite & sprite_shot, Sprite & sprite_grenade);

void UpdateHeroFrame(Hero & hero, float & time);

void DrawHero(RenderWindow & window, Hero & hero);

void DrawInventoryText(RenderWindow & window, vector<Inventory> & inventoryList, Hero & hero, View & view, Text & text);

void CheckLoot(Hero & hero, vector<Loot> & lootList, vector<Inventory> & inventoryList, Sprite & items);