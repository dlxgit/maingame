#pragma once
#include <SFML/Graphics.hpp>
#include "declarations.h"
#include "map.h"
#include "shot.h"
#include "hero.h"
#include "level.h"
#include "npc.h"
#include "zombie.h"
#include "loot.h"
#include "explosion.h"
#include "sprites.h"
#include "view.h"

using namespace sf;
using namespace std;

struct Door
{
	Texture texture;
	Sprite sprite;
	Vector2f pos;
	bool isOpened;
};

struct FinalDoor //not using yet
{
	Texture texture;
	Sprite sprite;
	Vector2f pos;
	int currentFrame;
	bool isOpened;
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
	//std::vector<House> houseList;
	std::vector<Explosion> explosionList;
	View view;
	GameState state;

	Font font;
	Text text;

	Sprites sprites;
	Texture explosionTexture;
	Sprite explosionSprite;

	Level lvl;
	vector<Object> solidObjects;
};

void InitializeGame(Game & game);

void CheckWindowClose(Game & game);

void BeginEvent(Game & game, View & view);

void EndGameEvent(Game & game, View & view);

void LevelFinishEvent(Game & game, View & view);

void DrawShots(RenderWindow & window, vector<Shot> & shotList, vector<Explosion> & explosionList, Hero & hero);

void DrawZombies(RenderWindow & window, vector<Zombie> & zombieList);

void DrawNpc(RenderWindow & window, vector<Npc> & npcList);

void CheckHeroBeastDamage(Hero & hero, vector<Zombie>::iterator & zombie, float & time);

bool IsShotCollision(vector<Zombie> & zombieList, NameItem & weapon, vector<Object> & objects, vector<Shot>::iterator  shot);

void UpdateShots(Game & game, Hero & hero, Sprite & sprite_explosion);

void UpdateZombies(vector<Zombie> & zombieList, Hero & hero, vector<Npc> & npcList, vector<Object> & objects, float & time);

void UpdateHero(Game & game);

void ProcessEvents(Game & game, Sprites & sprites);

void Render(Game & game);

void CheckSpawnZombiesAndLoot(Game & game, Sprite & items, Sprite & sprite_zombie);

void DrawMap(Game & game, Sprite & mapSprite);

void DrawBar(RenderWindow & window, vector<Inventory> & inventoryList, Hero & hero, View & view, Text & text, Sprites & sprites);

void StartGame(Game * game);

void DestroyGame(Game * game);