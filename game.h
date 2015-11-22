#pragma once
#include <SFML/Graphics.hpp>
#include "map.h"
#include "shot.h"
#include "hero.h"

using namespace sf;
using namespace std;

const int W_WIDTH = 1280;
const int W_HEIGHT = 1024;

enum GameState
{
	START_GAME,
	PLAY,
	LEVEL_FINISH,
	END_GAME,
	RESTART,
};

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

enum NpcState
{
	NPC_LIVING,
	NPC_DEAD,
	NPC_SURVIVED,
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
	float spawnTime;
	float dirChangeTime;
};

enum NpcType
{
	PHOTOGRAPHS,
	BABY,
	TEACHER,
	GIRL,
	DOG,
	SOLDIER,
	SEARCHER,
	COOK,	
};

struct Npc
{
	Vector2f pos;
	Texture texture;
	Sprite sprite;
	int health;
	NpcState state;
	float currentFrame;
	NpcType type;
};

struct FinalDoor
{
	Texture texture;
	Sprite sprite;
	int currentFrame;
};

struct Grenade
{
	Vector2f startPos;
	Vector2f distance;
	float startTime;
	bool isExploded;
};

struct House
{
	Texture texture;
	Sprite sprite;
	Vector2f pos;
};

struct Door
{
	Texture texture;
	Sprite sprite;
	Vector2f pos;
	bool isOpened;
};

struct FinalDoor1
{
	Texture texture;
	Sprite sprite;
	Vector2f pos;
	int currentFrame;
	bool isOpened;
};

struct Explosion
{
	Vector2f pos;
	float currentFrame;
	Sprite sprite;
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
	std::vector<Npc> npcList;
	std::vector<Door> doorList;
	std::vector<House> houseList;

	Font font;
	Text text;

	GameState state;

	std::vector<Explosion> explosionList;
	Texture explosionTexture;
	Sprite explosionSprite;
};


//.CPP

void InitializeNpc(Game & game, Texture & texture_npc)
{
	Npc npc;
	npc.currentFrame = 0;
	npc.health = 20;
	npc.texture = texture_npc;
	npc.sprite.setTexture(texture_npc);
	npc.state = NPC_LIVING;

	npc.type = PHOTOGRAPHS;
	npc.pos = { 5 * STEP, 8 * STEP };
	npc.sprite.setPosition(npc.pos);
	game.npcList.push_back(npc);

	npc.type = BABY;
	npc.pos = { 50 * STEP, 10 * STEP };
	npc.sprite.setPosition(npc.pos);
	game.npcList.push_back(npc);

	npc.type = TEACHER;
	npc.pos = { 10 * STEP, 15 * STEP };
	npc.sprite.setPosition(npc.pos);
	game.npcList.push_back(npc);

	npc.type = DOG;
	npc.pos = { 53 * STEP, 15 * STEP };
	npc.sprite.setPosition(npc.pos);
	game.npcList.push_back(npc);

	npc.type = SOLDIER;
	npc.pos = { 28 * STEP, 23 * STEP };
	npc.sprite.setPosition(npc.pos);
	game.npcList.push_back(npc);

	npc.type = SEARCHER;
	npc.pos = { 55 * STEP, 4 * STEP };
	npc.sprite.setPosition(npc.pos);
	game.npcList.push_back(npc);

	npc.type = COOK;
	npc.pos = { 20 * STEP, 15 * STEP };
	npc.sprite.setPosition(npc.pos);
	game.npcList.push_back(npc);
	npc.type = GIRL;
	npc.pos = { 15 * STEP, 6 * STEP };
	npc.sprite.setPosition(npc.pos);
	game.npcList.push_back(npc);
}

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
	game.state = START_GAME;
}

void CheckWindowClose(Game & game)
{
	Event event;
	while (game.window->pollEvent(event))
	{
		if (Keyboard::isKeyPressed(Keyboard::Escape) || event.type == Event::Closed)
		{
			game.window->close();
		}
	}
}

void BeginEvent(Game & game, View & view)
{
	Vector2f posView = view.getCenter();

	game.text.setString("It's time to rescue neighbors!");
	game.text.setPosition(posView.x - 140, posView.y - 100);
	game.window->draw(game.text);

	game.text.setString("Press Enter to play");
	game.text.setPosition(posView.x - 100, posView.y - 40);
	game.window->draw(game.text);

	if (Keyboard::isKeyPressed(Keyboard::Return))
	{
		game.state = PLAY;
	}
	CheckWindowClose(game);
}

void EndGameEvent(Game & game, View & view)
{
	Vector2f posView = view.getCenter();

	game.text.setString("GAME OVER");
	game.text.setPosition(posView.x - 40, posView.y - 100);
	game.window->draw(game.text);
	CheckWindowClose(game);
	/*
	std::ostringstream playerScoreString;
	playerScoreString << (snake->size - 1) * 10;
	game.text.setString("SCORE: " + playerScoreString.str());
	game.text.setPosition(240, 250);
	game.window.draw(game.text);
	

	game.text.setString("Press Enter to restart");
	game.text.setPosition(160, 350);
	game.window.draw(game.text);
	*/

	if (Keyboard::isKeyPressed(Keyboard::Return))
	{
		game.state = PLAY;
	}
}

void LevelFinishEvent(Game & game, View & view)
{
	Vector2f posView = view.getCenter();

	game.text.setString("LEVEL HAS FINISHED!");
	game.text.setPosition(posView.x - 100, posView.y - 100);
	game.window->draw(game.text);
	CheckWindowClose(game);
}


//movespeed of objects
const int STEP_HERO = 5;
const int STEP_HERO_HULK = 7;
const int STEP_ZOMBIE = 2;
const int STEP_ZOMBIE_ACTIVE = 3;
const int STEP_SHOT = 12;
const int STEP_GRENADE = 5;

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
const int NPC_MAX_HEALTH = 50;

//time 
const int BEAST_MAX_TIME = 30;
const float HERO_BEAST_ATTACK_TIME = 0.7;
const float WEAPON_RELOAD_TIME = 1.5;
const int TIME_GAME_STEP = 10;
const float SCREEN_UPDATE_TIME = 15;
const float GRENADE_MAX_TIME = 1;  //throwtime
const int ZOMBIE_DIR_CHANGE_TIME = 5;

//item quantity settings
const int AMMO_PACKS = 4;
const int MAX_AMMO[7] = { 1,12,30,1,1,1,1 };

//counts
const int MAX_NUMBER_OF_NEIGHBORS = 8;


//names
const string ITEM_NAMES[7] = { "drink", "pistol", "rifle", "ammo", "key", "mixture", "grenade"};