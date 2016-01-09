#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "declarations.h"
#include "hero.h"
#include "npc.h"

#include "throwing.h"


using namespace sf;
using namespace std;


struct Enemy
{
	EnemyType type;
	EnemyState state;
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
	bool isNear; 
	float spawnTime;
	float dirChangeTime;
	bool isAttack;
	bool isAction;
	float spawnDelay;
	
	bool isStop;
	float last_action_time;
	float actionTime;
	float actionDuration;
	
	float attackCooldown;
};

void ComputeThrownItemPosition(Sprite & sprite, Direction dir, float time, bool isHero);

void SpawnEnemy(vector<Enemy> & zombieList, float time, int posX, int posY, Sprites & sprites, const EnemyType &  type);

void CheckNpcDeath(vector<Npc> & npcList, Enemy & enemy);

Direction ComputeRandDir(Enemy & enemy);

void CheckEnemyDir(Enemy & enemy, float &time);

void CheckEnemyFollow(Enemy & enemy, Hero & hero);

void UpdateEnemyPosition(Enemy & enemy);

bool IsIntersectWithHero(Sprite & enemy, Sprite & hero);

bool IsEnemyNearHero(Hero & hero, Enemy & enemy);

void ComputeEnemyDirection(Enemy  & enemy, Vector2f & heroPos);

void ComputeEnemyAttackFrame(Enemy & enemy);

void UpdateEnemyAttack(Hero & hero, Enemy & enemy, const float & time);

void UpdateEnemyFrame(Enemy & enemy, float & time);

void ResetEnemySpawnTime(vector<Enemy> & zombieList, float & time);

bool IsIntersectWithOtherEnemy(vector<Enemy> & zombieList, int & index);

void EnemyMoveRandom(vector<Enemy> & zombieList);

Throwing CreateThrowing(Sprite & object, Sprite & target,  string type, Sprite & throwingSprite, float & time);

void SpawnEnemyRandomly(vector<Enemy>&zombieList, vector<Object> & objects, int zombiesRemaining, float & time, Sprites & sprites);

void CheckEnemyDir(float & time, Enemy & enemy);

void CheckEnemyExplosion(vector<Explosion> & explosionList, vector<Enemy> & zombieList);

void DrawEnemies(RenderWindow & window, vector<Enemy> & zombieList);

void DeleteEnemyList(vector<Enemy> & zombies);