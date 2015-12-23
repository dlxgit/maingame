#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "declarations.h"
#include "hero.h"
#include "npc.h"

using namespace sf;
using namespace std;


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
	bool isNear; 
	float spawnTime;
	float dirChangeTime;
};


void ZombieSpawn(vector<Zombie> & zombieList, float time, int posX, int posY, Sprite & sprite_zombie);

void CheckNpcDeath(vector<Npc> & npcList, Zombie & zombie);

Direction ComputeRandDir(Zombie & zombie);

void ZombieCheckDir(Zombie & zombie, float &time);

void ZombieCheckFollow(Zombie & zombie, Hero & hero);

void ZombieUpdatePosition(Zombie & zombie);

bool IsZombieNearHero(Hero & hero, Zombie & zombie);

void ComputeZombieDirection(Zombie  & zombie, Vector2f & heroPos);

void ZombieUpdateAttack(Hero & hero, Zombie & zombie, const float & time);

void ZombieUpdateSprite(Zombie & zombie);

void ZombieMoveRandom(vector<Zombie> & zombieList);

void SpawnZombieRandomly(vector<Zombie>&zombieList, vector<Object> & objects, int zombiesRemaining, float & time, Sprite & sprite_zombie);

void ZombieCheckDir(float & time, Zombie & zombie);

void CheckZombieExplosion(vector<Explosion> & explosionList, vector<Zombie> & zombieList);

void DeleteZombieList(vector<Zombie> & zombies);