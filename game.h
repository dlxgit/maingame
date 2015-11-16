#pragma once
#include <SFML/Graphics.hpp>
#include "map.h"

#include "hero.h"
using namespace sf;
using namespace std;



enum NameItem
{
	DRINK,
	PISTOL,
	RIFLE,
	AMMO,
	KEY,
	MIXTURE,
	GRENADE,
};


enum ZombieState
{
	NOTSPAWNED,  //cant move (raising from the ground)
	ACTIVE,
	DEAD,      //cant move (health = 0)
	EXPLODED  //if animation has finished (for deleting from list)
};


struct Inventory
{
	NameItem name;
	int quantity;
	int current;
	Texture texture;
	Sprite sprite;
};

struct Loot
{
	NameItem name;
	int quantity;
	Vector2f pos;
	Texture texture;
	Sprite sprite;
	bool isDrawn;
};


struct Zombie
{
	Vector2f pos;
	int health;
	Direction dir;
	Direction dirLast;
	bool follow;
	float attack_time;
	Texture texture;
	Sprite sprite;
	bool collision;
	float currentFrame;
	ZombieState state;
	bool isNear;  //side of zombie position relatively to hero
};

struct Shot
{
	Vector2f pos;
	int distance;
	Direction dir;
	Sprite sprite;
};

struct Npc
{
	Vector2f pos;
	Texture texture;
	Sprite sprite;
	int health;
};


struct Game
{
	Hero * hero;
	float time;
	RenderWindow * window;
	std::vector<Inventory> inventoryList;
	std::vector<Loot> lootList;
	std::vector<Shot> shotList;
	std::vector<Zombie> zombieList;
	std::vector<Npc> NpcList;
};

const int W_WIDTH = 1280;
const int W_HEIGHT = 1024;
//.CPP


void InitializeGame(Game & game)
{
	game.time = 0;
	game.window = new RenderWindow(VideoMode(W_WIDTH, W_HEIGHT), "Shoot and Run");

	Inventory inventory;
	inventory.name = PISTOL;
	inventory.current = 0;
	inventory.quantity = 7;
	inventory.texture.loadFromFile("images/items.png");
	inventory.sprite.setTexture(inventory.texture);
	inventory.sprite.setTextureRect(IntRect(32, 0, 32, 32));
	game.inventoryList.push_back(inventory);
	game.inventoryList[0].sprite.setTexture(inventory.texture);
}

//movespeed of objects
const int STEP_HERO = 6;
const int STEP_HERO_HULK = 8;
const int STEP_ZOMBIE = 2;
const int STEP_ZOMBIE_ACTIVE = 2;
const int STEP_SHOT = 12;

//damage of objects
const int ZOMBIE_DAMAGE = 30;
const int DMG_ITEM[7] = { 0, 35, 50,0, 0,0,100 };
const int HERO_BEAST_DAMAGE = 100;

//distance
const int SHOT_MAX_DISTANCE = 400;
const int ZOMBIE_VISION_DISTANCE = 300;

//health of objects
const int ZOMBIE_MAX_HP = 100;
const int HP_PER_DRINK = 30;

//time 
const int BEAST_MAX_TIME = 30;
const float HERO_BEAST_ATTACK_TIME = 0.7;
const float WEAPON_RELOAD_TIME = 1.5;
const int TIME_GAME_STEP = 10;

//item quantity settings
const int AMMO_PACKS = 4;
const int MAX_AMMO[7] = { 1,12,30,1,1,1,1 };

//names
const string ITEM_NAMES[7] = { "drink", "pistol", "rifle", "ammo", "key", "mixture", "grenade"};