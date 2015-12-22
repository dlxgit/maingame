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

void CheckNpcDeath(vector<Npc> & npcList, vector<Zombie>::iterator & zombie);

Direction ComputeRandDir(vector<Zombie>::iterator & zombie);

void ZombieCheckDir(vector<Zombie>::iterator & zombie, float &time);

void ZombieCheckFollow(vector<Zombie>::iterator & zombie, Hero & hero);

void ZombieUpdatePosition(vector<Zombie>::iterator & zombie);

bool IsZombieNearHero(Hero & hero, vector<Zombie>::iterator & zombie);

void ComputeZombieDirection(vector<Zombie>::iterator zombie, Vector2f & heroPos);

void ZombieUpdateAttack(Hero & hero, vector<Zombie>::iterator zombie, const float& time);

void ZombieUpdateSprite(vector<Zombie>::iterator & zombie);

void ZombieUpdateAttack(Hero & hero, vector<Zombie>::iterator zombie, float & time);

void ZombieMoveRandom(vector<Zombie> & zombieList);

void SpawnZombieRandomly(vector<Zombie>&zombieList, vector<Object> & objects, int zombiesRemaining, float & time, Sprite & sprite_zombie);

void ZombieCheckDir(float & time, vector<Zombie>::iterator & zombie);

void CheckZombieExplosion(vector<Explosion> & explosionList, vector<Zombie> & zombieList);

void DeleteZombieList(vector<Zombie> & zombies);