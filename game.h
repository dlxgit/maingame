#pragma once
#include <SFML/Graphics.hpp>
#include "declarations.h"
#include "shot.h"
#include "hero.h"
#include "level.h"
#include "npc.h"
#include "enemy.h"
#include "loot.h"
#include "explosion.h"
#include "sprites.h"
#include "view.h"
#include "text.h"

#include "minimap.h"

#include "throwing.h"

#include "boss.h"
#include "audio.h"

using namespace sf;
using namespace std;


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
	View view;
	std::vector<Inventory> inventoryList;
	std::vector<Loot> lootList;
	std::vector<Shot> shotList;
	std::vector<Enemy> zombieList;
	std::vector<Npc> npcList;
	std::vector<Explosion> explosionList; 
	std::vector<Throwing> throwingList;

	GameState state;

	Font font;
	Text text;

	Sprites sprites;
	AudioResources audio;

	Texture explosionTexture;
	Sprite explosionSprite;

	Level lvl;
	vector<Object> allObjects;
	vector<Object> solidObjects;

	MiniMap miniMap;

	int level;
	Boss boss;
};

void InitializeGame(Game & game);

void CheckWindowClose(Game & game);

void BeginEvent(Game & game);

void EndGameEvent(Game & game);

void LevelFinishEvent(Game & game);

void GameFinishEvent(Game & game);

void DrawShots(RenderWindow & window, vector<Shot> & shotList, vector<Explosion> & explosionList, Hero & hero);

void CheckHeroBeastDamage(Hero & hero, Enemy & enemy, float & time);

Vector2f ComputeSpriteNewPosition(Sprite & sprite, Direction & dir, const float & speed);

bool IsShotCollision(vector<Enemy> & zombieList, Hero & hero, vector<Object> & objects, Shot & shot);

void UpdateShots(Game & game, Hero & hero, Sprite & sprite_explosion);

bool IsCollisionWithMap(FloatRect & spriteRect, Direction & dir, vector<Object> &objects);

void UpdateAudio(Game & game);

void UpdateEnemies(Game & game);

void CheckEventNpc(vector<Npc> & npcList, Hero & hero, MiniMap & miniMap);

void UpdateSpritePosition(Sprite & sprite, Direction & dir, const float & speed, vector<Object> & solidObjects);

void UpdateHero(Game & game);

void UpdateBossPosition(Boss & boss, vector<Object> & solidObjects);

void ProcessEvents(Game & game, Sprites & sprites);

void Render(Game & game);

void CheckSpawnEnemyAndLoot(Game & game);

void CheckGameOver(Game & game);

void DrawBar(RenderWindow & window, vector<Inventory> & inventoryList, Hero & hero, View & view, Text & text, Sprites & sprites);

void UpdateBossEvent(Boss & boss, Hero & hero, float & time);

void UpdateBoss(Game & game);

void StartGame(Game * game);

void DeleteGame(Game * game);