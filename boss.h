#pragma once
#include "declarations.h"
#include "sprites.h"
#include "explosion.h"

struct Boss
{
	Sprite sprite;
	Texture texture;
	Direction dir;
	int health;
	float currentFrame;

	BossState state;
	BossEvent eventType;

	float spawnTime;
	float eventstartTime;
	bool isAttack;
	bool isEvent;
	int eventCount;

	Direction lastSide;  

	int shotCounter;

	bool isFollow;

	float speed;

	int spawnedEnemies;

	bool wasCharged;

	float followForShootTime;
	float shootStartTime;

	float lastShootTime;
	//BOSS_CHANGE_EVENT_TIME

	Sprite bar;
	Sprite indicator;
	
	bool isCommonZombie;
};

void InitializeBoss(Boss & boss, Sprites & sprites, float & time);

void UpdateBossFrame(Boss & boss);

void CheckBossExplosion(Boss & boss, vector<Explosion> & explosionList,int level);

bool IsReadyToShoot(Boss & boss, float & time);

bool IsBossNearMapCenter(Sprite & boss);

void ComputeBossDirection(Boss & boss, FloatRect & rect, Sprite & hero);

bool IsBossAbleToShoot(Sprite & hero, Sprite & boss);

FloatRect GetBossCollisionRect(Sprite & sprite);

bool IsFootRectIntersectWithHero(Sprite & hero, FloatRect boss);

bool IsReachedHero(Sprite & hero, Sprite & boss);

void UpdateBossMoveSpeed(Boss & boss);

void DrawBossBar(RenderWindow & window, Boss & boss, const Vector2f & viewPos);

void DrawBoss(RenderWindow & window, Sprite & boss);